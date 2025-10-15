#include "driver/gpio.h"
#include "esp_log.h"
#include "esp_sleep.h"
#include "freertos/FreeRTOS.h"
#include "freertos/projdefs.h"
#include "freertos/task.h"
#include <unistd.h>

#define LED_PIN GPIO_NUM_6

static const char *TAG = "Main";

void blink_led(void *params) {
	while (true) {
		gpio_set_level(LED_PIN, 1);
		vTaskDelay(pdMS_TO_TICKS(125));
		gpio_set_level(LED_PIN, 0);
		vTaskDelay(pdMS_TO_TICKS(125));
	}
}

void handle_sleep(void *params) {
	
	while (true) {	
		vTaskDelay(pdMS_TO_TICKS(4000));
		ESP_LOGI(TAG, "I am awake for now");
		vTaskDelay(pdMS_TO_TICKS(2000));
		ESP_LOGI(TAG, "I am going to sleep...");
		// Ensure all the data in the buffer is sent down to the UART driver
		// Otherwise the 'I am going to sleep...' message will not be fully sent
		fflush(stdout);
		fsync(fileno(stdout));

		// Start sleeping
		esp_light_sleep_start();

		/*

		Deep sleep instead of light sleep:
		'esp_deep_sleep_start();'

		This would act similar to a restart upon wakeup, so the
		'I have just woken up' message is never displayed.


		Important note:

		Using deep sleep saves the most power, but every time the device wakes
		up, it has to go through a full boot process, wasting energy. Running a
		'Deep-sleep wake stub' lets the MCU jump straight into a critical task
		and skip most of the boot sequence, therefore it wakes up faster, uses
		less power, and only initializes the peripherals it actually needs.

		More info about wake stubs:
		https://docs.espressif.com/projects/esp-idf/en/v5.5.1/esp32c6/api-guides/deep-sleep-stub.html

		*/

		// This message will only be seen after waking up
		ESP_LOGI(TAG, "I have just woken up");
	}
}

void app_main(void) {

	gpio_config_t led_config = {
		.pin_bit_mask = (1ULL << LED_PIN),
		.mode = GPIO_MODE_OUTPUT,
		.pull_up_en = 0,
		.pull_down_en = 0,
		.intr_type = GPIO_INTR_DISABLE,
	};
	gpio_config(&led_config);

	// Wake up 5 seconds after entering sleep
	esp_sleep_enable_timer_wakeup(5000000); // uS

	xTaskCreate(blink_led, "Blink LED", 2048, NULL, 1, NULL);
	xTaskCreate(handle_sleep, "Handle Sleep", 2048, NULL, 1, NULL);
}
