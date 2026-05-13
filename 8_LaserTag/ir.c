#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"

#include "esp_check.h"
#include "esp_log.h"

#include "driver/rmt_tx.h"
#include "driver/rmt_rx.h"
#include "driver/rmt_encoder.h"

#include "ir.h"

#define IR_TX_GPIO_NUM 5
#define IR_RX_GPIO_NUM 4
#define IR_RESOLUTION_HZ 1000000
#define IR_CARRIER_HZ 38000

#define IR_MAX_SYMBOLS 32
#define IR_QUEUE_LEN 4

#define START_ON_US 3000
#define START_OFF_US 1500
#define BIT_ON_US 500
#define BIT_0_OFF_US 500
#define BIT_1_OFF_US 1500
#define END_ON_US 500
#define END_OFF_US 500

#define DECODE_MARGIN_US 250

static const char *TAG = "IR";

static rmt_channel_handle_t s_tx_channel;
static rmt_channel_handle_t s_rx_channel;
static rmt_encoder_handle_t s_copy_encoder;
static QueueHandle_t s_rx_queue;
static rmt_symbol_word_t s_rx_symbols[IR_MAX_SYMBOLS];

static inline bool in_range(uint32_t value, uint32_t expected)
{
    return value > expected - DECODE_MARGIN_US && value < expected + DECODE_MARGIN_US;
}

static uint8_t checksum4(uint16_t data12)
{
    uint8_t a = (data12 >> 0) & 0x0F;
    uint8_t b = (data12 >> 4) & 0x0F;
    uint8_t c = (data12 >> 8) & 0x0F;
    return (a ^ b ^ c) & 0x0F;
}

static uint16_t packet_to_raw(const ir_packet_t *packet)
{
    uint16_t raw = 0;
    raw |= ((uint16_t)packet->player_id & 0x0F) << 0;
    raw |= ((uint16_t)packet->payload & 0xFF) << 4;
    raw |= ((uint16_t)checksum4(raw) & 0x0F) << 12;
    return raw;
}

static bool raw_to_packet(uint16_t raw, ir_packet_t *packet)
{
    uint16_t data12 = raw & 0x0FFF;
    uint8_t received_checksum = (raw >> 12) & 0x0F;

    if (received_checksum != checksum4(data12))
    {
        return false;
    }

    packet->player_id = (raw >> 0) & 0x0F;
    packet->payload = (raw >> 4) & 0xFF;
    return true;
}

static void build_symbols(uint16_t raw, rmt_symbol_word_t *symbols, size_t *symbol_count)
{
    size_t index = 0;

    symbols[index++] = (rmt_symbol_word_t){
        .level0 = 1,
        .duration0 = START_ON_US,
        .level1 = 0,
        .duration1 = START_OFF_US,
    };

    for (int bit = 0; bit < 16; bit++)
    {
        bool one = raw & (1 << bit);

        symbols[index++] = (rmt_symbol_word_t){
            .level0 = 1,
            .duration0 = BIT_ON_US,
            .level1 = 0,
            .duration1 = one ? BIT_1_OFF_US : BIT_0_OFF_US,
        };
    }

    symbols[index++] = (rmt_symbol_word_t){
        .level0 = 1,
        .duration0 = END_ON_US,
        .level1 = 0,
        .duration1 = END_OFF_US,
    };

    *symbol_count = index;
}

static bool decode_symbols(const rmt_symbol_word_t *symbols, size_t symbol_count, ir_packet_t *packet)
{
    if (symbol_count != 18)
    {
        return false;
    }

    if (!in_range(symbols[0].duration0, START_ON_US) || !in_range(symbols[0].duration1, START_OFF_US))
    {
        return false;
    }

    uint16_t raw = 0;

    for (int bit = 0; bit < 16; bit++)
    {
        const rmt_symbol_word_t *s = &symbols[1 + bit];

        if (!in_range(s->duration0, BIT_ON_US))
        {
            return false;
        }

        if (in_range(s->duration1, BIT_1_OFF_US))
        {
            raw |= 1 << bit;
        }
        else if (in_range(s->duration1, BIT_0_OFF_US))
        {
            raw &= ~(1 << bit);
        }
        else
        {
            return false;
        }
    }

    return raw_to_packet(raw, packet);
}

static bool rx_done_callback(rmt_channel_handle_t channel,
                             const rmt_rx_done_event_data_t *edata,
                             void *user_data)
{
    BaseType_t high_task_wakeup = pdFALSE;
    QueueHandle_t queue = (QueueHandle_t)user_data;

    xQueueSendFromISR(queue, edata, &high_task_wakeup);

    return high_task_wakeup == pdTRUE;
}

esp_err_t ir_init(void)
{
    rmt_tx_channel_config_t tx_cfg = {
        .clk_src = RMT_CLK_SRC_DEFAULT,
        .gpio_num = IR_TX_GPIO_NUM,
        .mem_block_symbols = 64,
        .resolution_hz = IR_RESOLUTION_HZ,
        .trans_queue_depth = 4,
    };
    ESP_RETURN_ON_ERROR(rmt_new_tx_channel(&tx_cfg, &s_tx_channel), TAG, "TX channel failed");

    rmt_carrier_config_t carrier_cfg = {
        .frequency_hz = IR_CARRIER_HZ,
        .duty_cycle = 0.33,
    };
    ESP_RETURN_ON_ERROR(rmt_apply_carrier(s_tx_channel, &carrier_cfg), TAG, "carrier failed");

    rmt_copy_encoder_config_t copy_cfg = {};
    ESP_RETURN_ON_ERROR(rmt_new_copy_encoder(&copy_cfg, &s_copy_encoder), TAG, "copy encoder failed");

    rmt_rx_channel_config_t rx_cfg = {
        .clk_src = RMT_CLK_SRC_DEFAULT,
        .gpio_num = IR_RX_GPIO_NUM,
        .mem_block_symbols = 64,
        .resolution_hz = IR_RESOLUTION_HZ,
    };
    ESP_RETURN_ON_ERROR(rmt_new_rx_channel(&rx_cfg, &s_rx_channel), TAG, "RX channel failed");

    s_rx_queue = xQueueCreate(IR_QUEUE_LEN, sizeof(rmt_rx_done_event_data_t));
    ESP_RETURN_ON_FALSE(s_rx_queue, ESP_ERR_NO_MEM, TAG, "RX queue failed");

    rmt_rx_event_callbacks_t callbacks = {
        .on_recv_done = rx_done_callback,
    };
    ESP_RETURN_ON_ERROR(rmt_rx_register_event_callbacks(s_rx_channel, &callbacks, s_rx_queue), TAG, "RX callback failed");

    ESP_RETURN_ON_ERROR(rmt_enable(s_tx_channel), TAG, "TX enable failed");
    ESP_RETURN_ON_ERROR(rmt_enable(s_rx_channel), TAG, "RX enable failed");

    rmt_receive_config_t rx_receive_cfg = {
        .signal_range_min_ns = 1000,
        .signal_range_max_ns = 5000000,
    };
    ESP_RETURN_ON_ERROR(rmt_receive(s_rx_channel, s_rx_symbols, sizeof(s_rx_symbols), &rx_receive_cfg), TAG, "initial receive failed");

    ESP_LOGI(TAG, "IR ready: TX GPIO=%d, RX GPIO=%d", IR_TX_GPIO_NUM, IR_RX_GPIO_NUM);
    return ESP_OK;
}

esp_err_t ir_send(const ir_packet_t *packet)
{
    ESP_RETURN_ON_FALSE(packet, ESP_ERR_INVALID_ARG, TAG, "packet is null");

    rmt_symbol_word_t tx_symbols[IR_MAX_SYMBOLS];
    size_t symbol_count = 0;

    build_symbols(packet_to_raw(packet), tx_symbols, &symbol_count);

    rmt_transmit_config_t tx_cfg = {
        .loop_count = 0,
    };

    ESP_RETURN_ON_ERROR(rmt_transmit(s_tx_channel,
                                     s_copy_encoder,
                                     tx_symbols,
                                     symbol_count * sizeof(rmt_symbol_word_t),
                                     &tx_cfg),
                        TAG, "transmit failed");

    return rmt_tx_wait_all_done(s_tx_channel, 100);
}

bool ir_receive(ir_packet_t *packet, uint32_t timeout_ms)
{
    rmt_rx_done_event_data_t rx_data;

    if (xQueueReceive(s_rx_queue, &rx_data, pdMS_TO_TICKS(timeout_ms)) != pdPASS)
    {
        return false;
    }

    bool ok = decode_symbols(rx_data.received_symbols, rx_data.num_symbols, packet);

    rmt_receive_config_t rx_receive_cfg = {
        .signal_range_min_ns = 1000,
        .signal_range_max_ns = 5000000,
    };
    ESP_ERROR_CHECK(rmt_receive(s_rx_channel, s_rx_symbols, sizeof(s_rx_symbols), &rx_receive_cfg));

    return ok;
}
