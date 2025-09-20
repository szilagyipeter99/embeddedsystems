#include "driver/gptimer.h"
#include "esp_attr.h"
#include "esp_log.h"
#include "esp_random.h"
#include <stdbool.h>
#include <stdio.h>
#include <unistd.h>

static const char *TAG = "Main";

volatile bool checkTempFlag = false;

// The callback function should return a bool
// Returning a true value is needed only if an RTOS task was unblocked
// inside the callback and a context switch should be performed
// Returning false essentially means no such change is needed
static bool IRAM_ATTR onTempTimer() {
	checkTempFlag = true;
	return false;
}

void app_main(void) {

	// Create and configure a timer
	gptimer_handle_t myTimer = NULL;
	gptimer_config_t timer_config = {
		.clk_src = GPTIMER_CLK_SRC_DEFAULT,
		.direction = GPTIMER_COUNT_UP,
		.resolution_hz = 1000000, // 1 Mhz (1 Tick/us)
		.intr_priority = 0,
	};
	gptimer_new_timer(&timer_config, &myTimer);
	// Would be safer this way:
	// ESP_ERROR_CHECK(gptimer_new_timer(&timer_config, &myTimer));

	// Add an alarm to the timer
	gptimer_alarm_config_t alarm_config = {
		.alarm_count = 1000000, // Period: 1s (Every millionth tick)
		.flags.auto_reload_on_alarm = true,
	};
	gptimer_set_alarm_action(myTimer, &alarm_config);

	// Configure a callback attached to the alarm	
	gptimer_event_callbacks_t myCallbacks = {
		.on_alarm = onTempTimer,
	};
	gptimer_register_event_callbacks(myTimer, &myCallbacks, NULL);
	
	// Important: Only one alarm per timer and one callback per alarm is allowed
	
	// Enable and start the timer
	gptimer_enable(myTimer);
	gptimer_start(myTimer);

	while (true) {

		if (checkTempFlag) {
			uint8_t randomTemp = esp_random() % 20 + 20;
			ESP_LOGI(TAG, "Temperature: %d °C", randomTemp);
			checkTempFlag = false;
		}

		// Any other task
	}
}
