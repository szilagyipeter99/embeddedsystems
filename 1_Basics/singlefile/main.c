#include "esp_log.h"
#include "esp_random.h"
#include <stdbool.h>
#include <unistd.h>

// Preprocessing directive to define a constant
#define COLD_THRESHOLD 20.0

static const char *TAG = "Main";

// Function prototypes
float readTemperature();
void evaluateTemperature(float temperature, float threshold);
int8_t getRoundedTemperature(float temperature);

// Main function
void app_main(void) {

	// These run once after booting
	ESP_LOGI(TAG, "Size of int: %d", sizeof(int));
	ESP_LOGI(TAG, "Size of int8_t: %d", sizeof(int8_t));
	ESP_LOGI(TAG, "Size of float: %d", sizeof(float));
	ESP_LOGI(TAG, "----------------------------");

	// Code inside this 'while' will run indefinitely
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
	// 0.5 without the 'f' would work, but it would default to double precision
	// (more memory, slower calculations)
}
