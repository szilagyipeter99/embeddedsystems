#include "esp_log.h"
#include "temp_monitor.h"
#include <stdbool.h>
#include <unistd.h>

#define COLD_THRESHOLD 20.0

static const char *TAG = "Main";

void app_main(void) {
	
	ESP_LOGI(TAG, "----------------------------");

	while (true) {
		float data = readTemperature();
		ESP_LOGI(TAG, "Sensor data: %.2f", data);
		int8_t roundedTemp = getRoundedTemperature(data);
		ESP_LOGI(TAG, "Rounded temperature: %d °C", roundedTemp);
		evaluateTemperature(data, COLD_THRESHOLD);
		ESP_LOGI(TAG, "----------------------------");
		sleep(2);
	}
}
