#include "driver/gptimer.h"
#include "esp_attr.h"
#include "esp_log.h"
#include "esp_random.h"
#include <unistd.h>

static const char *TAG = "Main";

volatile bool check_temp_flag = false;

// The callback function should return a bool
// Returning a true value is needed only if an RTOS task was unblocked
// inside the callback and a context switch should be performed
// Returning false essentially means no such change is needed
static bool IRAM_ATTR on_temp_timer() {
	check_temp_flag = true;
	return false;
}

void app_main(void) {

	// Create and configure a timer
	gptimer_handle_t my_timer = NULL;
	gptimer_config_t timer_config = {
		.clk_src = GPTIMER_CLK_SRC_DEFAULT,
		.direction = GPTIMER_COUNT_UP,
		.resolution_hz = 1000000, // 1 Mhz (1 Tick/us)
		.intr_priority = 0,
	};
	gptimer_new_timer(&timer_config, &my_timer);
	// Would be safer this way:
	// ESP_ERROR_CHECK(gptimer_new_timer(&timer_config, &my_timer));

	// Add an alarm to the timer
	gptimer_alarm_config_t alarm_config = {
		.alarm_count = 1000000, // Period: 1s (Every millionth tick)
		.flags.auto_reload_on_alarm = true,
	};
	gptimer_set_alarm_action(my_timer, &alarm_config);

	// Configure a callback attached to the alarm
	gptimer_event_callbacks_t my_callbacks = {
		.on_alarm = on_temp_timer,
	};
	gptimer_register_event_callbacks(my_timer, &my_callbacks, NULL);

	// Important: Only one alarm per timer and one callback per alarm is allowed

	// Enable then start the timer
	gptimer_enable(my_timer);
	gptimer_start(my_timer);

	while (true) {

		if (check_temp_flag) {
			uint8_t random_temp = esp_random() % 20 + 20;
			ESP_LOGI(TAG, "Temperature: %d °C", random_temp);
			check_temp_flag = false;
		}

		// Any other task

		// Feed the watchdog
		usleep(10000);
	}
}
