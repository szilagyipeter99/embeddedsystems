#include "esp_log.h"
#include "mqtt_client.h"

#include "app_defs.h"

static esp_mqtt_client_handle_t my_mqtt_client = NULL;

static const char *TAG = "MQTT Connector";

/*

Full list of events:
https://docs.espressif.com/projects/esp-idf/en/v5.5.1/esp32c6/api-reference/protocols/mqtt.html#events

*/

static void mqtt_event_handler(void *arg, esp_event_base_t base, int32_t id, void *data) {
	switch (id) {
	case (MQTT_EVENT_BEFORE_CONNECT):
		ESP_LOGI(TAG, "The client is initialized, connecting to the broker...");
		break;
	case (MQTT_EVENT_CONNECTED):
		xEventGroupSetBits(my_event_group, MQTT_CONNECTED_BIT);
		break;
	case (MQTT_EVENT_DISCONNECTED):
		xEventGroupSetBits(my_event_group, MQTT_FAILED_BIT);
		break;
	case (MQTT_EVENT_SUBSCRIBED):
		// ...
		break;
	case (MQTT_EVENT_PUBLISHED):
		xEventGroupSetBits(my_event_group, MQTT_PUBLISHED_BIT);
		break;
	case (MQTT_EVENT_DATA):
		// ...
		break;
	case (MQTT_EVENT_ERROR):
		ESP_LOGE(TAG, "The client has encountered an error");
		break;
	default:
		break;
	}
}

esp_err_t mqtt_init() {

	esp_mqtt_client_config_t my_mqtt_config = {
		.broker.address.uri = "mqtt://petermqtt.local",
	};

	my_mqtt_client = esp_mqtt_client_init(&my_mqtt_config);

	if (my_mqtt_client == NULL) {
		return ESP_FAIL;
	}

	ESP_ERROR_CHECK(esp_mqtt_client_register_event(my_mqtt_client, -1, &mqtt_event_handler, NULL));
	ESP_ERROR_CHECK(esp_mqtt_client_start(my_mqtt_client));

	vTaskDelay(pdMS_TO_TICKS(250));

	EventBits_t bits = xEventGroupWaitBits(my_event_group, MQTT_CONNECTION_BITMASK, pdFALSE, pdFALSE, pdMS_TO_TICKS(10000));

	if (bits & MQTT_CONNECTED_BIT) {
		ESP_LOGI(TAG, "The client has successfully connected to the broker");
		return ESP_OK;
	} else if (bits & MQTT_FAILED_BIT) {
		return ESP_FAIL;
	} else {
		return ESP_ERR_TIMEOUT;
	}
}

void mqtt_publish_btn_pressed() {

	esp_mqtt_client_publish(my_mqtt_client, "my_home/living_room/led7/set", "ON", 0, 1, 0);

	EventBits_t bits = xEventGroupWaitBits(my_event_group, MQTT_PUBLISHED_BIT, pdFALSE, pdFALSE, pdMS_TO_TICKS(5000));

	if (bits & MQTT_PUBLISHED_BIT) {
		ESP_LOGI(TAG, "The client has successfully published the message");
		xEventGroupClearBitsFromISR(my_event_group, MQTT_PUBLISHED_BIT);
	} else {
		ESP_LOGE(TAG, "The client has failed to publish the message");
	}
}

/*

TODO: The client should subscribe to 'my_home/living_room/bed_lamp/status' which
can be used to read the state and possibly other parameters from the LED lamp

*/