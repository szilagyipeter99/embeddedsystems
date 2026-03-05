#include "driver/ledc.h"
#include <unistd.h>

#define LED_PIN GPIO_NUM_6

void app_main(void) {

	/*

	- There are configurable timers and channels. The 7 available channels are
	  technically GPIOs, all of them have their own duty cycle. There are 4
	  selectable timers that are configurable with different resolutions and
	  frequencies. Every channel should use one of the configured timers.

	- ESP32C6 only supports 'LEDC_LOW_SPEED_MODE'. Unfortunately, ESP-IDF
	  doesn't provide a built-in shortcut or default for it, so it must be
	  explicitly passed every time.

	*/

	// Configure a LEDC timer
	ledc_timer_config_t pwm_config = {
		.speed_mode = LEDC_LOW_SPEED_MODE,
		.duty_resolution = LEDC_TIMER_12_BIT,
		.timer_num = LEDC_TIMER_0,
		.freq_hz = 5000,
		.clk_cfg = LEDC_AUTO_CLK,
	};

	// Configure a LEDC channel
	ledc_channel_config_t channel_config = {
		.gpio_num = LED_PIN,
		.speed_mode = LEDC_LOW_SPEED_MODE,
		.channel = LEDC_CHANNEL_0,
		.intr_type = LEDC_INTR_DISABLE,
		.timer_sel = LEDC_TIMER_0,
		.duty = 0,
		.hpoint = 0,
	};

	ledc_timer_config(&pwm_config);
	ledc_channel_config(&channel_config);

	while (true) {
		// Set the LED brightness to 100%
		ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0, 4095);
		ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0);
		sleep(1);
		// Set the LED brightness to ~75%
		ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0, 3072);
		ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0);
		sleep(1);
		// Set the LED brightness to ~50%
		ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0, 2048);
		ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0);
		sleep(1);
		// Set the LED brightness to ~25%
		ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0, 1024);
		ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0);
		sleep(1);
	}
}
