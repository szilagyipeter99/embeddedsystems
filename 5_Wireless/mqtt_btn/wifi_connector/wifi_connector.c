#include "esp_log.h"
#include "esp_wifi.h"

#include "app_defs.h"

#define MAX_RETRY_ATTEMPTS 5

static uint8_t retry_cntr = 0;

static esp_ip4_addr_t ipv4;

static const char *TAG = "Wi-Fi Connector";

static void wifi_event_handler(void *arg, esp_event_base_t base, int32_t id,
							   void *data) {
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
		if (retry_cntr < MAX_RETRY_ATTEMPTS) {
			ESP_LOGI(TAG, "Retrying to connect to Wi-Fi network...");
			esp_wifi_connect();
			retry_cntr++;
		} else {
			xEventGroupSetBits(my_event_group, WIFI_FAILED_BIT);
		}
		break;
	case (IP_EVENT_STA_GOT_IP):
		ipv4 = ((ip_event_got_ip_t *)data)->ip_info.ip;
		ESP_LOGI(TAG, "Got IPv4: " IPSTR, IP2STR(&ipv4));
		retry_cntr = 0;
		xEventGroupSetBits(my_event_group, WIFI_CONNECTED_BIT);
		break;
	default:
		ESP_LOGI(TAG, "Unhandled event occured");
		break;
	}
}

void wifi_init_phase() {

	ESP_ERROR_CHECK(esp_netif_init());

	ESP_ERROR_CHECK(esp_event_loop_create_default());

	// This has built-in error check
	esp_netif_create_default_wifi_sta();

	wifi_init_config_t my_init_config = WIFI_INIT_CONFIG_DEFAULT();

	esp_log_level_set("wifi", ESP_LOG_WARN);

	ESP_ERROR_CHECK(esp_wifi_init(&my_init_config));

	ESP_ERROR_CHECK(
		esp_event_handler_register(WIFI_EVENT, -1, &wifi_event_handler, NULL));
	ESP_ERROR_CHECK(
		esp_event_handler_register(IP_EVENT, -1, &wifi_event_handler, NULL));
}

void wifi_config_phase() {

	ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));

	wifi_config_t my_sta_config = {
		.sta =
			{
				.ssid = "SzPDevNetwork",
				.password = "tZAPBMIe3Vn",
				.threshold.authmode = WIFI_AUTH_WPA3_PSK,
			},
	};

	ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &my_sta_config));
}

esp_err_t wifi_start_phase() {

	ESP_ERROR_CHECK(esp_wifi_start());

	vTaskDelay(pdMS_TO_TICKS(250));

	EventBits_t bits =
		xEventGroupWaitBits(my_event_group, WIFI_CONNECTION_BITMASK, pdFALSE,
							pdFALSE, pdMS_TO_TICKS(10000));

	if (bits & WIFI_CONNECTED_BIT) {
		ESP_LOGI(TAG, "Connected to Wi-Fi network");
		return ESP_OK;
	} else if (bits & WIFI_FAILED_BIT) {
		return ESP_FAIL;
	} else {
		return ESP_ERR_TIMEOUT;
	}
}
