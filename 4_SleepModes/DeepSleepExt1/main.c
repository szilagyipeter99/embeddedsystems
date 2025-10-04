#include "driver/gptimer.h"
#include "driver/rtc_io.h"
#include "esp_log.h"
#include "esp_random.h"
#include "esp_sleep.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#define BTN_PIN GPIO_NUM_4
#define LED_PIN GPIO_NUM_6

// This value is stored in LP memory
RTC_DATA_ATTR static uint8_t random_value = 0;

static const char *TAG = "Main";

volatile bool sleep_flag = false;

static bool IRAM_ATTR on_sleep_timer() {
	sleep_flag = true;
	return false;
}

void blink_led(void *param) {

	gpio_config_t led_config = {
		.pin_bit_mask = (1ULL << LED_PIN),
		.mode = GPIO_MODE_OUTPUT,
		.pull_up_en = 0,
		.pull_down_en = 0,
		.intr_type = GPIO_INTR_DISABLE,
	};
	gpio_config(&led_config);

	while (true) {
		gpio_set_level(LED_PIN, 1);
		vTaskDelay(pdMS_TO_TICKS(125));
		gpio_set_level(LED_PIN, 0);
		vTaskDelay(pdMS_TO_TICKS(125));
	}
}

void handle_sleep(void *param) {

	// To wake up from deep sleep using one or multuple GPIO pins (Ext1), they
	// have to be configured as RTC IOs (Pins 0-7 on the ESP32C6)
	rtc_gpio_init(BTN_PIN);
	rtc_gpio_set_direction(BTN_PIN, RTC_GPIO_MODE_INPUT_ONLY);
	rtc_gpio_pulldown_dis(BTN_PIN);
	rtc_gpio_pullup_en(BTN_PIN);

	// Configure a 5 second timer
	gptimer_handle_t my_timer = NULL;
	gptimer_config_t timer_config = {
		.clk_src = GPTIMER_CLK_SRC_DEFAULT,
		.direction = GPTIMER_COUNT_UP,
		.resolution_hz = 1000000,
		.intr_priority = 0,
	};
	gptimer_new_timer(&timer_config, &my_timer);
	gptimer_alarm_config_t alarm_config = {
		.alarm_count = 5000000,
		.flags.auto_reload_on_alarm = true, // This is not needed
	};
	gptimer_set_alarm_action(my_timer, &alarm_config);
	gptimer_event_callbacks_t my_callbacks = {
		.on_alarm = on_sleep_timer,
	};
	gptimer_register_event_callbacks(my_timer, &my_callbacks, NULL);
	gptimer_enable(my_timer);
	gptimer_start(my_timer);

	// Configure Ext1 wakeup
	esp_sleep_enable_ext1_wakeup_io((1ULL << BTN_PIN), ESP_EXT1_WAKEUP_ANY_LOW);

	while (true) {
		// Send the MCU to sleep if the interrupt was triggered
		if (sleep_flag) {
			// No need to put flag back to false because its value is lost
			esp_deep_sleep_start();
		}
	}
}

void app_main(void) {

	// A new random number between 1 and 100 is generated if the variable stored
	// in LP memory does not have a non-zero value yet
	if (random_value == 0) {
		random_value = esp_random() % 100 + 1;
	}
	ESP_LOGI(TAG, "The random value: %d", random_value);

	xTaskCreate(blink_led, "Blink LED", 2048, NULL, 1, NULL);
	xTaskCreate(handle_sleep, "Handle Sleep", 2048, NULL, 1, NULL);
}
