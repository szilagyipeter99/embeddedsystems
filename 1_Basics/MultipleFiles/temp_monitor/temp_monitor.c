#include "esp_log.h"
#include "esp_random.h"

static const char *TAG = "TempMonitor";

float readTemperature() {
	float randomTemp;
	randomTemp = esp_random() % 20 + 10 + (esp_random() % 100) / 100.0;
	return randomTemp;
}

void evaluateTemperature(float temperature, float threshold) {
	if (temperature >= threshold) {
		ESP_LOGI(TAG, "Normal temperature.");
	} else {
		ESP_LOGI(TAG, "Low temperature.");
	}
}

int8_t getRoundedTemperature(float temperature) {
	return (int8_t)(temperature + 0.5f);
}
