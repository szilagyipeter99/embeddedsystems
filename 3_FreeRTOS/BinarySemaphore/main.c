#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#define LED_1_PIN GPIO_NUM_6
#define LED_2_PIN GPIO_NUM_7

#define LED_MASK ((1ULL << LED_1_PIN) | (1ULL << LED_2_PIN))

// Semaphore handle
SemaphoreHandle_t my_sem;

void blink_led_1(void *params) {
	while (true) {
		// Take the semaphore when available while waiting as much as possible
		xSemaphoreTake(my_sem, portMAX_DELAY);
		for (int i = 0; i < 5; i++) {
			gpio_set_level(LED_1_PIN, 1);
			vTaskDelay(pdMS_TO_TICKS(500));
			gpio_set_level(LED_1_PIN, 0);
			vTaskDelay(pdMS_TO_TICKS(500));
		}
		xSemaphoreGive(my_sem);
		// Wait at least 1 tick to let other tasks take the semaphore
		vTaskDelay(1);
	}
}

void blink_led_2(void *params) {
	while (true) {
		xSemaphoreTake(my_sem, portMAX_DELAY);
		for (int i = 0; i < 10; i++) {
			gpio_set_level(LED_1_PIN, 1);
			vTaskDelay(pdMS_TO_TICKS(125));
			gpio_set_level(LED_1_PIN, 0);
			vTaskDelay(pdMS_TO_TICKS(125));
		}
		xSemaphoreGive(my_sem);
		vTaskDelay(1);
	}
}

void app_main(void) {

	gpio_config_t led_config = {
		.pin_bit_mask = LED_MASK,
		.mode = GPIO_MODE_OUTPUT,
		.pull_up_en = 0,
		.pull_down_en = 0,
		.intr_type = GPIO_INTR_DISABLE,
	};
	gpio_config(&led_config);

	// Create a binary semaphore
	my_sem = xSemaphoreCreateBinary();

	/*

	Counting semaphore: xSemaphoreCreateCounting()
	Mutex: xSemaphoreCreateMutex()

	*/

	xTaskCreate(blink_led_1, "Blink #1", 2048, NULL, 1, NULL);
	xTaskCreate(blink_led_2, "Blink #2", 2048, NULL, 1, NULL);

	// Make the semaphore available
	xSemaphoreGive(my_sem);
}
