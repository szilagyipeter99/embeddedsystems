#include "driver/gpio.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#define LED_1_PIN GPIO_NUM_6
#define LED_2_PIN GPIO_NUM_7

#define LED_MASK ((1ULL << LED_1_PIN) | (1ULL << LED_2_PIN))

static const char *TAG = "Main";

// Task to blink LED_1
void blink_led_1(void *params) {
	while (true) {
		gpio_set_level(LED_1_PIN, 1);
		vTaskDelay(pdMS_TO_TICKS(500));
		gpio_set_level(LED_1_PIN, 0);
		vTaskDelay(pdMS_TO_TICKS(500));
	}
}

// Task to blink LED_2
void blink_led_2(void *params) {
	while (true) {
		gpio_set_level(LED_2_PIN, 1);
		vTaskDelay(pdMS_TO_TICKS(125));
		gpio_set_level(LED_2_PIN, 0);
		vTaskDelay(pdMS_TO_TICKS(125));
	}
}

void app_main(void) {

	ESP_LOGI(TAG, "Tick period (ms): %u", (unsigned)portTICK_PERIOD_MS);
	ESP_LOGI(TAG, "Tick rate (Hz): %u", (unsigned)configTICK_RATE_HZ);

	// Configure the LEDs
	gpio_config_t led_config = {
		.pin_bit_mask = LED_MASK,
		.mode = GPIO_MODE_OUTPUT,
		.pull_up_en = 0,
		.pull_down_en = 0,
		.intr_type = GPIO_INTR_DISABLE,
	};
	gpio_config(&led_config);

	// Create FreeRTOS tasks
	xTaskCreate(blink_led_1, // Task function
				"Blink #1",	 // Name of the task (for logging/debugging)
				2048, // Stack size in words (measure usage for best practice)
				NULL, // Parameters to pass (NULL: a pointer to nowhere)
				1,	  // Task priority
				NULL  // Task handle to interact with it
	);
	xTaskCreate(blink_led_2, "Blink #2", 2048, NULL, 1, NULL);
}
