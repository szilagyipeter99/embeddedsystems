#include "esp_log.h"
#include "esp_wifi.h"
#include "nvs_flash.h"

/*

There is a built-in FreeRTOS method to track events called an event group:

- To synchronize tasks with basic booleans a mutex or spin-loop would be
needed so that tasks can wait safely. With event groups, a task can block
efficiently until a condition is met.

- Event groups are easily scalable. If more states have to be tracked,
developers can just create more bits. The logic does not have to be redesigned.

*/

static EventGroupHandle_t my_event_group = NULL;

#define CONNECTED_BIT BIT0 // = 0x00000001 = 0b00000000000000000000000000000001
#define FAILED_BIT BIT1	   // = 0x00000002 = 0b00000000000000000000000000000010
#define CONNECTION_BITMASK (CONNECTED_BIT | FAILED_BIT)

#define MAX_RETRY_ATTEMPTS 5

static uint8_t retry_cntr = 0;

static esp_ip4_addr_t ipv4;

static const char *TAG = "Main";

/*

The 'event_handler()' function handles predefined events by the Wi-Fi driver

Full list of events:
https://docs.espressif.com/projects/esp-idf/en/v5.5.1/esp32c6/api-guides/wifi.html#esp32-c6-wi-fi-event-description

*/

static void event_handler(void *arg, esp_event_base_t base, int32_t id, void *data) {
	switch (id) {
	case (WIFI_EVENT_STA_START):
		ESP_LOGI(TAG, "Wi-Fi started, connecting to AP...");
		esp_wifi_connect();
		break;
	case (WIFI_EVENT_STA_STOP):
		ESP_LOGI(TAG, "Wi-Fi stopped");
		break;
	case (WIFI_EVENT_STA_CONNECTED):
		ESP_LOGI(TAG, "Wi-Fi connected, waiting for IP address...");
		break;
	case (WIFI_EVENT_STA_DISCONNECTED):
		ESP_LOGE(TAG, "Wi-Fi disconnected");
		// Try to reconnect multple times
		if (retry_cntr < MAX_RETRY_ATTEMPTS) {
			ESP_LOGI(TAG, "Retrying to connect to Wi-Fi network...");
			esp_wifi_connect();
			retry_cntr++;
		} else {
			xEventGroupSetBits(my_event_group, FAILED_BIT);
		}
		break;
	case (IP_EVENT_STA_GOT_IP):
		ipv4 = ((ip_event_got_ip_t *)data)->ip_info.ip;
		ESP_LOGI(TAG, "Got IPv4: " IPSTR, IP2STR(&ipv4));
		retry_cntr = 0;
		xEventGroupSetBits(my_event_group, CONNECTED_BIT);
		break;
	default:
		ESP_LOGI(TAG, "Unhandled event occured.");
		break;
	}
}

/*

There are 3 main phases to start using Wi-Fi on the ESP32C6:

- Init phase
- Configuration phase
- Start phase

Recommended handling of phases:
https://docs.espressif.com/projects/esp-idf/en/v5.5.1/esp32c6/api-guides/wifi.html#esp32-c6-wi-fi-station-general-scenario

*/

static void init_phase() {

	my_event_group = xEventGroupCreate();

	esp_netif_init();
	esp_event_loop_create_default();
	esp_netif_create_default_wifi_sta();
	wifi_init_config_t my_init_config = WIFI_INIT_CONFIG_DEFAULT();
	esp_wifi_init(&my_init_config);

	// To fire on any 'event_id' either -1 or 'ESP_EVENT_ANY_ID' has to be set
	esp_event_handler_register(WIFI_EVENT, -1, &event_handler, NULL);
	esp_event_handler_register(IP_EVENT, -1, &event_handler, NULL);
}

static void config_phase() {

	esp_wifi_set_mode(WIFI_MODE_STA);

	wifi_config_t my_sta_config = {
		.sta =
			{
				.ssid = "...",
				.password = "...",
				// Set weakest accepted auth mode
				.threshold.authmode = WIFI_AUTH_WPA3_PSK,
			},
	};

	esp_wifi_set_config(WIFI_IF_STA, &my_sta_config);

	/*

	'esp_wifi_set_config()' also saves the configuration into NVS and can be
	loaded from flash by the 'esp_wifi_get_config()' function

	*/
}

static void start_phase() {

	esp_wifi_start();

	vTaskDelay(pdMS_TO_TICKS(500));

	EventBits_t bits = xEventGroupWaitBits(my_event_group, CONNECTION_BITMASK, pdFALSE, pdFALSE, portMAX_DELAY);

	if (bits & CONNECTED_BIT) {
		ESP_LOGI(TAG, "Connected to Wi-Fi network");
	} else if (bits & FAILED_BIT) {
		ESP_LOGE(TAG, "Failed to connect to Wi-Fi network");
	}
}

void app_main(void) {

	/*

	NVS = Non-Volatile Storage

	A small key-value database in flash memory that persists across reboots,
	ESP-IDF uses it internally to remember configuration data

	If the NVS partition is full or contains invalid data, it should be erased
	and initialized again

	Most functions in ESP-IDF return ESP_OK, ESP_FAIL or other types of error
	codes, that is why most of them are 'esp_err_t' type. There is a built-in
	macro 'ESP_ERROR_CHECK()' that can check these returned codes and if there
	is an error, logs it and aborts. Its usage is only shown for the NVS in this
	project but most of the Wi-Fi commands must use it.

	*/

	esp_err_t ret = nvs_flash_init();

	if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
		ESP_ERROR_CHECK(nvs_flash_erase());
		ret = nvs_flash_init();
	}

	ESP_ERROR_CHECK(ret);

	init_phase();
	config_phase();
	start_phase();
}
