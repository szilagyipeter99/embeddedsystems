#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#define LED_1_PIN GPIO_NUM_6
#define LED_2_PIN GPIO_NUM_7

#define LED_MASK ((1ULL << LED_1_PIN) | (1ULL << LED_2_PIN))

// Create a structure for the LEDs and call this type of data 'blink_t'
// Same as how 'gpio_config_t' and others are defined by Espressif
typedef struct {
	int pin;
	int interval;
} blink_t;

// Use the custom 'blink_t' type to define two LEDs
blink_t blink_1 = {
	.pin = LED_1_PIN,
	.interval = 425,
};
blink_t blink_2 = {
	.pin = LED_2_PIN,
	.interval = 775,
};

// Task to handle both LEDs based on the input parameters
void blink_led_x(void *params) {
	// Cast back the input parameter from 'void *' to 'blink_t *'
	blink_t *blink_data = (blink_t *)params;
	while (true) {
		gpio_set_level(blink_data->pin, 1);
		vTaskDelay(pdMS_TO_TICKS(blink_data->interval));
		gpio_set_level(blink_data->pin, 0);
		vTaskDelay(pdMS_TO_TICKS(blink_data->interval));
	}
}

void app_main(void) {

	// Configure the LEDs
	gpio_config_t led_config = {
		.pin_bit_mask = LED_MASK,
		.mode = GPIO_MODE_OUTPUT,
		.pull_up_en = 0,
		.pull_down_en = 0,
		.intr_type = GPIO_INTR_DISABLE,
	};
	gpio_config(&led_config);

	// xTaskCreate() expects the parameter to be of type 'void *'
	xTaskCreate(blink_led_x, "Blink #1", 2048, (void *)&blink_1, 1, NULL);
	xTaskCreate(blink_led_x, "Blink #2", 2048, (void *)&blink_2, 1, NULL);
}
