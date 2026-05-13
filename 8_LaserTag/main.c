#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "esp_random.h"

#include "ir.h"

#define PLAYER_ID 1
#define BTN_PIN GPIO_NUM_4

static const char *TAG = "Main";

void app_main(void)
{
    gpio_config_t btn_config = {
        .pin_bit_mask = 1ULL << BTN_PIN,
        .mode = GPIO_MODE_INPUT,
        .pull_up_en = 1,
    };

    ESP_ERROR_CHECK(gpio_config(&btn_config));

    ESP_ERROR_CHECK(ir_init());

    bool last_btn_state = true;
    bool curr_btn_state;

    uint8_t shot_counter = 0;

    ir_packet_t received;

    while (true)
    {
        curr_btn_state = gpio_get_level(BTN_PIN);
        if (curr_btn_state == false && last_btn_state == true)
        {
            shot_counter++;
            ir_packet_t packet = {
                .player_id = PLAYER_ID,
                .payload = shot_counter,
            };
            ESP_LOGI(TAG, "SENT: player=%d payload=%d", packet.player_id, packet.payload);
            ESP_ERROR_CHECK(ir_send(&packet));
        }
        if (ir_receive(&received, 10))
        {
            ESP_LOGI(TAG, "RECEIVED: player=%d payload=%d", received.player_id, received.payload);
        }
        last_btn_state = curr_btn_state;
        vTaskDelay(pdMS_TO_TICKS(20));
    }
}
