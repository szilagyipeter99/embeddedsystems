#include "driver/gpio.h"
#include "esp_attr.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#define BTN_1_PIN GPIO_NUM_4
#define BTN_2_PIN GPIO_NUM_5
#define LED_1_PIN GPIO_NUM_6
#define LED_2_PIN GPIO_NUM_7

#define BTN_MASK ((1ULL << BTN_1_PIN) | (1ULL << BTN_2_PIN))
#define LED_MASK ((1ULL << LED_1_PIN) | (1ULL << LED_2_PIN))

// Semaphore handles
SemaphoreHandle_t my_sem_1;
SemaphoreHandle_t my_sem_2;

// Functions to handle the ISRs
static void IRAM_ATTR handle_first_isr() {
	xSemaphoreGiveFromISR(my_sem_1, NULL);
}
static void IRAM_ATTR handle_second_isr() {
	xSemaphoreGiveFromISR(my_sem_2, NULL);
}

void blink_led_1(void *params) {
	while (true) {
		xSemaphoreTake(my_sem_1, portMAX_DELAY);
		for (int i = 0; i < 5; i++) {
			gpio_set_level(LED_1_PIN, 1);
			vTaskDelay(pdMS_TO_TICKS(500));
			gpio_set_level(LED_1_PIN, 0);
			vTaskDelay(pdMS_TO_TICKS(500));
		}
	}
}

void blink_led_2(void *params) {
	while (true) {
		xSemaphoreTake(my_sem_2, portMAX_DELAY);
		for (int i = 0; i < 10; i++) {
			gpio_set_level(LED_1_PIN, 1);
			vTaskDelay(pdMS_TO_TICKS(125));
			gpio_set_level(LED_1_PIN, 0);
			vTaskDelay(pdMS_TO_TICKS(125));
		}
	}
}

void app_main(void) {

	gpio_config_t btn_config = {
		.pin_bit_mask = BTN_MASK,
		.mode = GPIO_MODE_INPUT,
		.pull_up_en = 1,
		.pull_down_en = 0,
		.intr_type = GPIO_INTR_NEGEDGE,
	};
	gpio_config(&btn_config);

	gpio_config_t led_config = {
		.pin_bit_mask = LED_MASK,
		.mode = GPIO_MODE_OUTPUT,
		.pull_up_en = 0,
		.pull_down_en = 0,
		.intr_type = GPIO_INTR_DISABLE,
	};
	gpio_config(&led_config);

	// Configure the ISRs
	gpio_install_isr_service(ESP_INTR_FLAG_IRAM);
	gpio_isr_handler_add(BTN_1_PIN, handle_first_isr, NULL);
	gpio_isr_handler_add(BTN_2_PIN, handle_second_isr, NULL);

	// Create binary semaphores
	my_sem_1 = xSemaphoreCreateBinary();
	my_sem_2 = xSemaphoreCreateBinary();

	xTaskCreate(blink_led_1, "Blink #1", 2048, NULL, 1, NULL);
	xTaskCreate(blink_led_2, "Blink #2", 2048, NULL, 1, NULL);
}

/*

Bouncing issue:
There might be multiple blink sequences after a single button press

Possible solution:
Hardware debounce or 'xTaskGetTickCountFromISR()' can count the ticks since the
last ISR and if it happened too soon, ignore it

*/
