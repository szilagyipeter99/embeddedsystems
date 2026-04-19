#include "esp_log.h"
#include "esp_wifi.h"
#include "nvs_flash.h"

#include "esp_https_ota.h"
#include "esp_http_client.h"
#include "esp_app_desc.h"

static EventGroupHandle_t my_event_group = NULL;

#define CONNECTED_BIT BIT0
#define FAILED_BIT BIT1
#define CONNECTION_BITMASK (CONNECTED_BIT | FAILED_BIT)

#define MAX_RETRY_ATTEMPTS 5

static uint8_t retry_cntr = 0;

static esp_ip4_addr_t ipv4;

static const char *TAG = "Main";

static void event_handler(void *arg, esp_event_base_t base, int32_t id, void *data) {
	switch (id) {
	// WIFI events
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
			xEventGroupSetBits(my_event_group, FAILED_BIT);
		}
		break;
	case (IP_EVENT_STA_GOT_IP):
		ipv4 = ((ip_event_got_ip_t *)data)->ip_info.ip;
		ESP_LOGI(TAG, "Got IPv4: " IPSTR, IP2STR(&ipv4));
		retry_cntr = 0;
		xEventGroupSetBits(my_event_group, CONNECTED_BIT);
		break;
	// HTTPS OTA events
	case ESP_HTTPS_OTA_START:
		ESP_LOGI(TAG, "OTA update started");
		break;
	case ESP_HTTPS_OTA_CONNECTED:
		ESP_LOGI(TAG, "Connected to OTA update server");
		break;
	case ESP_HTTPS_OTA_WRITE_FLASH:
		ESP_LOGD(TAG, "Writing to flash: %d", *(int *)event_data);
		break;
	case ESP_HTTPS_OTA_UPDATE_BOOT_PARTITION:
		ESP_LOGI(TAG, "Boot partition updated, next partition: %d", *(esp_partition_subtype_t *)event_data);
		break;
	case ESP_HTTPS_OTA_FINISH:
		ESP_LOGI(TAG, "OTA update finished");
		break;
	case ESP_HTTPS_OTA_ABORT:
		ESP_LOGI(TAG, "OTA update aborted");
		break;
	default:
		ESP_LOGI(TAG, "Unhandled event occured.");
		break;
	}
}

static void init_phase() {

	my_event_group = xEventGroupCreate();

	esp_netif_init();
	esp_event_loop_create_default();
	esp_netif_create_default_wifi_sta();
	wifi_init_config_t my_init_config = WIFI_INIT_CONFIG_DEFAULT();
	esp_wifi_init(&my_init_config);

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
				.threshold.authmode = WIFI_AUTH_WPA3_PSK,
			},
	};

	esp_wifi_set_config(WIFI_IF_STA, &my_sta_config);

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

static const char* get_running_version() {
	
    esp_app_desc_t *desc = esp_app_get_description();
    return desc->version;
}

static bool is_update_available(void) {
    
	char buffer[32] = {0};

    esp_http_client_config_t config = {
        .url = "http://petermqtt.local/latest_version",
        .event_handler = http_event_handler,
    };

    esp_http_client_handle_t ver_check_client = esp_http_client_init(&config);

    if (esp_http_client_perform(ver_check_client) != ESP_OK)
    {
        ESP_LOGE(TAG, "Failed to fetch version file");
        esp_http_client_cleanup(ver_check_client);
        return false;
    }

    int32_t len = esp_http_client_read_response(ver_check_client, buffer, sizeof(buffer));
    esp_http_client_cleanup(client);

    if (len <= 0) {
        ESP_LOGE(TAG, "Empty version response");
        return false;
    }

    buffer[len] = '\0';

    const char *current = get_running_version();

	// If versions do not match, perform an update
	// (Semantic version comparison should be implemented)
    if (strcmp(buffer, current) != 0) {
        ESP_LOGI(TAG, "New firmware detected!");
        return true;
    }

    return false;
}

static void do_firmware_update() {
	
    esp_http_client_config_t config = {
        .url = "http://petermqtt.local/latest_firmware.bin",
    };
	
    esp_https_ota_config_t ota_config = {
        .http_config = &config,
    };

	esp_event_handler_register(ESP_HTTPS_OTA_EVENT, -1, &event_handler, NULL);
	
    esp_err_t ret = esp_https_ota(&ota_config);
	
    if (ret == ESP_OK) {
		ESP_LOGI(TAG, "Updated firmware successfully");
        esp_restart();
    } else {
		ESP_LOGI(TAG, "Failed to perform OTA update");
    }
}

static void give_random_value(uint8_t *val_ptr) {
	*val_ptr = esp_random() % 256; 
}

void app_main(void) {

	esp_err_t ret = nvs_flash_init();

	if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
		ESP_ERROR_CHECK(nvs_flash_erase());
		ret = nvs_flash_init();
	}

	ESP_ERROR_CHECK(ret);

	init_phase();
	config_phase();
	start_phase();

	if (is_firmware_update_available()) {
    	do_firmware_update();
	}

	uint8_t some_value = 0;

	while (true) {
		give_random_value(&some_value);
		ESP_LOGI(TAG, "New value: %d", some_value);
		sleep(1);
	}

}
