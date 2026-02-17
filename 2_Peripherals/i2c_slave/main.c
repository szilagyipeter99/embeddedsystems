#include "driver/i2c_slave.h"
#include "esp_log.h"
#include <unistd.h>

#define SDA_PIN GPIO_NUM_4
#define SCL_PIN GPIO_NUM_5

#define SLAVE_ADDR 0x69

static const char *TAG = "Main";

volatile bool data_received_flag = false;

static bool on_data_receive() {
	data_received_flag = true;
    return false;
}

void app_main(void) {

    // Configure a new I2C master
    i2c_slave_config_t slave_config = {
        .i2c_port = I2C_NUM_0,
        .sda_io_num = SDA_PIN,
        .scl_io_num = SCL_PIN,
        .clk_source = I2C_CLK_SRC_DEFAULT,
        .slave_addr = SLAVE_ADDR,
        .addr_bit_len = I2C_ADDR_BIT_LEN_7,
        .intr_priority = 0,
        .send_buf_depth = 100,
    };

    // Allocate a handle for an I2C slave
    i2c_slave_dev_handle_t slave_handle;

    i2c_new_slave_device(&slave_config, &slave_handle);

    // Configure a callback attached to the receive job
    i2c_slave_event_callbacks_t my_callbacks = {
		.on_recv_done = on_data_receive,
	};
    i2c_slave_register_event_callbacks(slave_handle, &my_callbacks, NULL);

    // Initialize a buffer
    uint8_t read_buf[2];

    // Receive new data
    // (non-blocking, initiates a new receive job and then returns, runs once)
    i2c_slave_receive(slave_handle, read_buf, 2);

    while (true) {

        if (data_received_flag) {
            // Build back the 16-bit number
			uint16_t some_value = ((uint16_t)read_buf[0] << 8) | read_buf[1];
            ESP_LOGI(TAG, "Recived value: %d", some_value);
            // Receive new data again 
            i2c_slave_receive(slave_handle, read_buf, 2);
			data_received_flag = false;
		}

        // Any other task

        // Feed the watchdog
		usleep(10000);
    }
}
