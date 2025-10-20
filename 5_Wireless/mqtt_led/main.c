#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "nvs_flash.h"

#include "app_defs.h"
#include "mqtt_connector.h"
#include "wifi_connector.h"

#define LED_PIN GPIO_NUM_6

EventGroupHandle_t my_event_group = NULL;

QueueHandle_t led_queue = NULL;

static void handle_led_task(void *params) {

	gpio_config_t led_config = {
		.pin_bit_mask = (1ULL << LED_PIN),
		.mode = GPIO_MODE_OUTPUT,
		.pull_up_en = 0,
		.pull_down_en = 0,
		.intr_type = GPIO_INTR_DISABLE,
	};
	ESP_ERROR_CHECK(gpio_config(&led_config));

	bool led_state = false;

	while (true) {
		xQueueReceive(led_queue, &led_state, portMAX_DELAY);
		gpio_set_level(LED_PIN, led_state);
	}
}

void app_main(void) {

	esp_err_t ret = nvs_flash_init();

	if (ret == ESP_ERR_NVS_NO_FREE_PAGES ||
		ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
		ESP_ERROR_CHECK(nvs_flash_erase());
		ret = nvs_flash_init();
	}

	ESP_ERROR_CHECK(ret);

	my_event_group = xEventGroupCreate();

	wifi_init_phase();
	wifi_config_phase();

	ESP_ERROR_CHECK(wifi_start_phase());

	led_queue = xQueueCreate(5, sizeof(int));

	ESP_ERROR_CHECK(mqtt_init());
	ESP_ERROR_CHECK(mqtt_subscribe_to_led());

	xTaskCreate(handle_led_task, "Handle LED", 2048, NULL, 1, NULL);
}