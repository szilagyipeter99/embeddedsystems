#include "esp_log.h"
#include "esp_random.h"
#include <unistd.h>

static const char *TAG = "Main";

// Write a new value to the received memory address
void give_random_value(uint8_t *val_ptr) { 
	*val_ptr = esp_random() % 256; 
}

void app_main(void) {

	// Create the variable
	uint8_t some_value = 0;

	while (true) {
		// The input is the memory address of the variable
		give_random_value(&some_value);
		ESP_LOGI(TAG, "New value: %d", some_value);
		sleep(1);
	}
}
