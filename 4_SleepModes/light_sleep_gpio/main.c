#include "driver/gpio.h"
#include "driver/uart.h"
#include "esp_sleep.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <string.h>
#include <unistd.h>

#define BTN_PIN GPIO_NUM_4
#define LED_PIN GPIO_NUM_6

#define BUFFER_SIZE 50
#define SLEEP_MSG "GO_SLEEP\r"

// Empty array for incoming (max. 50 character long) messages
static char msg_buffer[BUFFER_SIZE];

/*

Each task configures the peripherals it uses, keeping 'app_main()' simple

*/

void blink_led(void *param) {

	gpio_config_t led_config = {
		.pin_bit_mask = (1ULL << LED_PIN),
		.mode = GPIO_MODE_OUTPUT,
		.pull_up_en = 0,
		.pull_down_en = 0,
		.intr_type = GPIO_INTR_DISABLE,
	};
	gpio_config(&led_config);

	while (true) {
		gpio_set_level(LED_PIN, 1);
		vTaskDelay(pdMS_TO_TICKS(125));
		gpio_set_level(LED_PIN, 0);
		vTaskDelay(pdMS_TO_TICKS(125));
	}
}

void handle_sleep(void *param) {

	gpio_config_t btn_config = {
		.pin_bit_mask = (1ULL << BTN_PIN),
		.mode = GPIO_MODE_INPUT,
		.pull_up_en = 1,
		.pull_down_en = 0,
		.intr_type = GPIO_INTR_DISABLE,
	};
	gpio_config(&btn_config);

	// Let the GPIO driver log its settings
	fflush(stdout);
	fsync(fileno(stdout));

	// Configure GPIO wakeup
	// Trigger on low level (internal pull-up enabled)
	gpio_wakeup_enable(BTN_PIN, GPIO_INTR_LOW_LEVEL);
	esp_sleep_enable_gpio_wakeup();

	// Simple UART configuration to read incoming messsages
	// (No flow control, no TX buffer, no event queue ...)
	uart_config_t uart_config = {
		.baud_rate = 115200,
		.data_bits = UART_DATA_8_BITS,
		.parity = UART_PARITY_DISABLE,
		.stop_bits = UART_STOP_BITS_1,
		.flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
		.source_clk = UART_SCLK_DEFAULT,
	};
	uart_driver_install(UART_NUM_0, 1024, 0, 0, NULL, 0);
	uart_param_config(UART_NUM_0, &uart_config);
	// Use the default pins for UART communication
	uart_set_pin(UART_NUM_0, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE,
				 UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);

	while (true) {
		// Wait for a message to arrive
		// ('uart_read_bytes' returns the length of the message)
		int len = uart_read_bytes(UART_NUM_0, msg_buffer, BUFFER_SIZE - 1, 5);

		/*

		5 ms (5 RTOS ticks right now) as the maximum time to wait until the
		requested number of bytes arrive might not be enough depending on the
		length of the message:

		Baud rate: 115200 bits per second
		UART frame: start bit + 8 message bits + stop bit = 10 bits (no parity)

		Time per byte = 10 / 115200 ≈ 0.087 ms

		So a 1000 byte message would take approximately 87 ms to transmit

		*/

		// Check if a message arrived ('uart_read_bytes' returns -1 on error)
		if (len > 0) {
			// Null-terminate (close) the message
			msg_buffer[len] = '\0';
			// Compare it to the sleep command to check if its correct
			if (strcmp(msg_buffer, SLEEP_MSG) == 0) {
				esp_light_sleep_start();
			}
		}

		// Feed the watchdog
		vTaskDelay(pdMS_TO_TICKS(10));
	}
}

void app_main(void) {

	xTaskCreate(blink_led, "Blink LED", 2048, NULL, 1, NULL);
	xTaskCreate(handle_sleep, "Handle Sleep", 2048, NULL, 1, NULL);
}
