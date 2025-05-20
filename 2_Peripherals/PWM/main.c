#include "driver/ledc.h"
#include "hal/ledc_types.h"
#include "soc/clk_tree_defs.h"
#include "soc/gpio_num.h"
#include <stdbool.h>
#include <unistd.h>

#define LED_PIN GPIO_NUM_12

void app_main(void) {

	ledc_timer_config_t pwm_config = {
		.speed_mode = LEDC_LOW_SPEED_MODE,
		.duty_resolution = LEDC_TIMER_12_BIT,
		.timer_num = LEDC_TIMER_0,
		.freq_hz = 5000,
		.clk_cfg = LEDC_AUTO_CLK,
	};
	ledc_timer_config(&pwm_config);

	ledc_channel_config_t channel_config = {
		.gpio_num = LED_PIN,
		.speed_mode = LEDC_LOW_SPEED_MODE,
		.channel = LEDC_CHANNEL_0,
		.intr_type = LEDC_INTR_DISABLE,
		.timer_sel = LEDC_TIMER_0,
		.duty = 0,
		.hpoint = 0,
	};
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
