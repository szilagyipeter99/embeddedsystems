#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#define LED_PIN GPIO_NUM_6

TaskHandle_t blink_led_handle;

// Basic task to blink an LED
void blink_led(void *params) {
	while (true) {
		gpio_set_level(LED_PIN, 1);
		vTaskDelay(pdMS_TO_TICKS(125));
		gpio_set_level(LED_PIN, 0);
		vTaskDelay(pdMS_TO_TICKS(125));
	}
}

// This task is responsible for suspending and resuming the other task
void suspend_resume(void *params) {
	while (true) {
		vTaskDelay(pdMS_TO_TICKS(3000));
		vTaskSuspend(blink_led_handle);
		vTaskDelay(pdMS_TO_TICKS(1000));
		vTaskResume(blink_led_handle);
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

	xTaskCreate(blink_led, "Blink", 2048, NULL, 1, &blink_led_handle);
	xTaskCreate(suspend_resume, "Suspend and Resume", 2048, NULL, 1, NULL);
}
