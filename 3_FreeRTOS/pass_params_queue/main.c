#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

// Message queue handle
QueueHandle_t my_queue;

static const char *TAG = "Main";

void send_msg(void *params) {
	uint32_t cntr = 0;
	while (true) {
		cntr++;
		// Wait for 0 ticks, then try to get a place in the queue
		if (xQueueSend(my_queue, &cntr, 0) == pdTRUE) {
			ESP_LOGI(TAG, "Sent: %lu", cntr);
		}
		// If new data could not be added to the queue
		else {
			ESP_LOGW(TAG, "Queue full, %lu is not in the queue.", cntr);
		}
		vTaskDelay(pdMS_TO_TICKS(1000)); // Try to send a message every 1s
	}
}

void receive_msg(void *params) {
	uint32_t received_val;
	while (true) {
		// Take an element from the queue
		xQueueReceive(my_queue, &received_val, portMAX_DELAY);
		ESP_LOGI(TAG, "Received: %lu", received_val);
		// Simulate long processing time
		vTaskDelay(pdMS_TO_TICKS(2250));
	}
}

void app_main(void) {

	// Create a queue which can hold 5 integers
	my_queue = xQueueCreate(5, sizeof(uint32_t));

	xTaskCreate(send_msg, "Send Message", 2048, NULL, 1, NULL);
	xTaskCreate(receive_msg, "Receive Message", 2048, NULL, 1, NULL);
}
