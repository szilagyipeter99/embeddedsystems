#include "esp_log.h"
#include "esp_random.h"

// A different tag to identify log messages from this file
static const char *TAG = "Temp. Monitor";

float read_temperature() {
	float random_temp = esp_random() % 20 + 10 + (esp_random() % 100) / 100.0;
	return random_temp;
}

int8_t get_rounded_temperature(float temperature) {
	return (int8_t)(temperature + 0.5f);
}

void evaluate_temperature(float temperature, float threshold) {
	if (temperature >= threshold) {
		ESP_LOGI(TAG, "Normal temperature.");
	} else {
		ESP_LOGW(TAG, "Low temperature.");
	}
}
