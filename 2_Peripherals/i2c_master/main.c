#include "driver/i2c_master.h"
#include "esp_random.h"
#include "esp_log.h"
#include <unistd.h>

#define SDA_PIN GPIO_NUM_4
#define SCL_PIN GPIO_NUM_5

#define SLAVE_ADDR 0x69

static const char *TAG = "Main";

void app_main(void) {

    // Configure a new I2C bus
    i2c_master_bus_config_t bus_config = {
        .i2c_port = I2C_NUM_0,
        .sda_io_num = SDA_PIN,
        .scl_io_num = SCL_PIN,
        .clk_source = I2C_CLK_SRC_DEFAULT,
        .glitch_ignore_cnt = 7,
        .intr_priority = 0,
        .flags.enable_internal_pullup = false,
    };

    // Allocate a handle for an I2C bus
    i2c_master_bus_handle_t bus_handle;

    i2c_new_master_bus(&bus_config, &bus_handle);

    // Configure a new I2C master
    i2c_device_config_t master_config = {
        .dev_addr_length = I2C_ADDR_BIT_LEN_7,
        .device_address = SLAVE_ADDR,
        .scl_speed_hz = 100000,
    };

    // Allocate a handle for an I2C master
    i2c_master_dev_handle_t master_handle;

    i2c_master_bus_add_device(bus_handle, &master_config, &master_config);

    // Initialize a variable and a buffer
    uint16_t some_value;
    uint8_t write_buf[2];

    while (true) {
        // Generate a large random number between 0 and 49999
        some_value = esp_random() % 50000;
        // Divide the 16-bit number into two bytes for transmission
        write_buf[0] = (some_value >> 8) & 0xFF;
        write_buf[1] = some_value & 0xFF;
        i2c_master_transmit(master_handle, write_buf, 2, 1000); // 1 sec. timeout
        ESP_LOGI(TAG, "Data sent: %d", some_value);
        sleep(2);
    }   
}
