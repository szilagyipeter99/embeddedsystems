#include <stdbool.h>
#include <unistd.h>

// Include the custom header
#include "temp_monitor.h"

// Including 'esp_log.h' is technically not needed here
// ('temp_monitor.h' already includes it)
#include "esp_log.h"

#define COLD_THRESHOLD 20.0f

static const char *TAG = "Main";

void app_main(void) {

	ESP_LOGI(TAG, "----------------------------");

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
