#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "nvs_flash.h"

#include "app_defs.h"
#include "mqtt_connector.h"
#include "wifi_connector.h"

#define BTN_PIN GPIO_NUM_4

EventGroupHandle_t my_event_group = NULL;
 
static void handle_btn_task(void *params) {

	gpio_config_t btn_config = {
		.pin_bit_mask = (1ULL << BTN_PIN),
		.mode = GPIO_MODE_INPUT,
		.pull_up_en = 1,
		.pull_down_en = 0,
		.intr_type = GPIO_INTR_DISABLE,
	};
	ESP_ERROR_CHECK(gpio_config(&btn_config));

	bool last_btn_state = true;
	bool curr_btn_state;

	while (true) {
		curr_btn_state = gpio_get_level(BTN_PIN);
		if (curr_btn_state == false && last_btn_state == true) {
			mqtt_publish_btn_pressed();
		}
		last_btn_state = curr_btn_state;
		vTaskDelay(pdMS_TO_TICKS(20));
	}
}

void app_main(void) {

	esp_err_t ret = nvs_flash_init();

	if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
		ESP_ERROR_CHECK(nvs_flash_erase());
		ret = nvs_flash_init();
	}

	ESP_ERROR_CHECK(ret);

	my_event_group = xEventGroupCreate();

	wifi_init_phase();
	wifi_config_phase();

	ESP_ERROR_CHECK(wifi_start_phase());

	/*

	Even after a successful connection the 'WIFI_EVENT_STA_DISCONNECTED'
	event could happen at any time (for example the AP loses power) and that
	should also be handled.

	*/

	ESP_ERROR_CHECK(mqtt_init());

	xTaskCreate(handle_btn_task, "Handle Button", 2048, NULL, 1, NULL);
}

/*

This project uses polling to detect a button press, which might introduce a
slight delay and usually considered bad practice. Now however a potential few
miliseconds of delay is acceptable as both Wi-Fi and MQTT inevitably introduce
delays anyway compared to local GPIO operations, therefore, basic polling is
sufficient instead of ISRs.

*/
