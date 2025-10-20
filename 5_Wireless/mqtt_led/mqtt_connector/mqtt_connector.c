#include "esp_log.h"
#include "mqtt_client.h"

#include "app_defs.h"

static esp_mqtt_client_handle_t my_mqtt_client = NULL;

static bool led_state = false;

static void mqtt_handle_received_data(void *data);

static const char *TAG = "MQTT Connector";

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
		xEventGroupSetBits(my_event_group, MQTT_SUBSCRIBED_BIT);
		break;
	case (MQTT_EVENT_PUBLISHED):
		// ...
		break;
	case (MQTT_EVENT_DATA):
		mqtt_handle_received_data(data);
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

esp_err_t mqtt_subscribe_to_led() {

	esp_mqtt_client_subscribe(my_mqtt_client, "my_home/living_room/led7/set", 1);

	EventBits_t bits = xEventGroupWaitBits(my_event_group, MQTT_SUBSCRIBED_BIT, pdFALSE, pdFALSE, pdMS_TO_TICKS(5000));

	if (bits & MQTT_SUBSCRIBED_BIT) {
		ESP_LOGI(TAG, "The client has successfully subscribed to the topic");
		return ESP_OK;
	} else {
		return ESP_ERR_TIMEOUT;
	}
}

static void mqtt_handle_received_data(void *data) {

	esp_mqtt_event_handle_t event = data;

	if (strncmp(event->topic, "my_home/living_room/led7/set", event->topic_len) == 0) {

		if (strncmp(event->data, "ON", event->data_len) == 0) {
			led_state = true;
			ESP_LOGI(TAG, "LED light turned ON");
		} else if (strncmp(event->data, "OFF", event->data_len) == 0) {
			led_state = false;
			ESP_LOGI(TAG, "LED light turned OFF");
		} else {
			ESP_LOGE(TAG, "Invalid event data");
			return;
		}

		xQueueSend(led_queue, &led_state, 0);
	}
}

/*

TODO: The client should publish its actual state and possibly other parameters
from the LED lamp to 'my_home/living_room/led7/status'

*/