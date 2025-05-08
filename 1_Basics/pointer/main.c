#include "esp_log.h"
#include "esp_random.h"
#include <stdbool.h>
#include <unistd.h>

static const char *TAG = "Main";

void give_random_value(uint8_t *val_ptr) { 
	*val_ptr = esp_random() % 256; 
}

void app_main(void) {

	uint8_t some_value = 0;

	while (true) {
		give_random_value(&some_value);
		ESP_LOGI(TAG, "New value: %d", some_value);
		sleep(1);
	}
}
