#include "esp_log.h"
#include "esp_random.h"
#include <stdbool.h>
#include <unistd.h>

// Preprocessing directive to define a float constant
// 20.0 without the 'f' would work, but it would default to double precision
// (8 bytes instead of 4, more memory, slower calculations)
#define COLD_THRESHOLD 20.0f

// Logging tag used to identify log output from this file
// 'static' here means the variable is only visible within this file
static const char *TAG = "Main";

// Function prototypes (to declare functions before they are used)
float read_temperature();
int8_t get_rounded_temperature(float temperature);
void evaluate_temperature(float temperature, float threshold);

// Main function
void app_main(void) {

	// These run once after booting
	ESP_LOGI(TAG, "Size of int: %d", sizeof(int));
	ESP_LOGI(TAG, "Size of int8_t: %d", sizeof(int8_t));
	ESP_LOGI(TAG, "Size of float: %d", sizeof(float));
	ESP_LOGI(TAG, "----------------------------");

	// Code inside this 'while' will run indefinitely
	while (true) {
		float temp_from_sensor = read_temperature();
		ESP_LOGI(TAG, "Sensor data: %.2f", temp_from_sensor);
		int8_t rounded_temp = get_rounded_temperature(temp_from_sensor);
		ESP_LOGI(TAG, "Rounded temperature: %d °C", rounded_temp);
		evaluate_temperature(temp_from_sensor, COLD_THRESHOLD);
		ESP_LOGI(TAG, "----------------------------");
		sleep(2);
	}
}

float read_temperature() {
	float random_temp;
	// Generate a pseudo-random temperature between 10.00 and 20.99
	random_temp = esp_random() % 20 + 10 + (esp_random() % 100) / 100.0;
	return random_temp;
}

int8_t get_rounded_temperature(float temperature) {
	return (int8_t)(temperature + 0.5f);
	// Example 1: 18.3 + 0.5 = 18.8, 18 after casting
	// Example 2: 6.7 + 0.5 = 7.2, 7 after casting
}

// 'void' because it does not return any value
void evaluate_temperature(float temperature, float threshold) {
	if (temperature >= threshold) {
		ESP_LOGI(TAG, "Normal temperature.");
	} else {
		ESP_LOGI(TAG, "Low temperature.");
	}
}
