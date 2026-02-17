#include "driver/gpio.h"
#include "esp_attr.h"
#include <unistd.h>

#define BTN_1_PIN GPIO_NUM_4
#define BTN_2_PIN GPIO_NUM_5
#define LED_1_PIN GPIO_NUM_6
#define LED_2_PIN GPIO_NUM_7

// GPIOs are configured using a bit mask
// 1ULL (unsigned long long) is 1 as a 64 bit unsigned int
// The bit mask is a list of zeros with ones at the indexes of the button
// numbers
/*
Example:
(1ULL << 5) = 0b0000000000000000000000000000000000000000000000000000000000100000
(1ULL << 7) = 0b0000000000000000000000000000000000000000000000000000000010000000
Bitwise OR  = 0b0000000000000000000000000000000000000000000000000000000010100000
Decimal     = 128 + 32 = 160
*/
#define BTN_MASK ((1ULL << BTN_1_PIN) | (1ULL << BTN_2_PIN))
#define LED_MASK ((1ULL << LED_1_PIN) | (1ULL << LED_2_PIN))

// Without being volatile, the compiler might cache the value of the variable in
// a register and never notice the ISR changed it
volatile bool first_blink = 0;
volatile bool second_blink = 0;

// IRAM_ATTR flag:
// Tells the compiler and linker to place the ISR in RAM instead of flash memory
// Ensures faster access, prevents optimizations from the compiler
static void IRAM_ATTR handle_first_isr() { first_blink = 1; }
static void IRAM_ATTR handle_second_isr() { second_blink = 1; }

void app_main(void) {

	// Configure both buttons
	gpio_config_t btn_config = {
		.pin_bit_mask = BTN_MASK,
		.mode = GPIO_MODE_INPUT,
		.pull_up_en = 1,
		.pull_down_en = 0,
		.intr_type = GPIO_INTR_NEGEDGE,
	};
	gpio_config(&btn_config);

	// Configure both LEDs
	gpio_config_t led_config = {
		.pin_bit_mask = LED_MASK,
		.mode = GPIO_MODE_OUTPUT,
		.pull_up_en = 0,
		.pull_down_en = 0,
		.intr_type = GPIO_INTR_DISABLE,
	};
	gpio_config(&led_config);

	// Configure the ISRs
	gpio_install_isr_service(ESP_INTR_FLAG_IRAM);
	gpio_isr_handler_add(BTN_1_PIN, handle_first_isr, NULL);
	gpio_isr_handler_add(BTN_2_PIN, handle_second_isr, NULL);

	while (true) {
		// This is executed as soon as 'first_blink' is true
		// and the CPU is available
		if (first_blink) {
			gpio_set_level(LED_1_PIN, 1);
			usleep(500000);
			gpio_set_level(LED_1_PIN, 0);
			usleep(500000);
			gpio_set_level(LED_1_PIN, 1);
			usleep(500000);
			gpio_set_level(LED_1_PIN, 0);
			usleep(500000);
			gpio_set_level(LED_1_PIN, 1);
			usleep(500000);
			gpio_set_level(LED_1_PIN, 0);
			first_blink = false;
		}
		// Same logic for 'second_blink' but with a for loop
		if (second_blink) {
			for (int i = 0; i < 3; ++i) {
				gpio_set_level(LED_2_PIN, 1);
				usleep(500000);
				gpio_set_level(LED_2_PIN, 0);
				usleep(500000);
			}
			second_blink = false;
		}

		// 10ms delay to feed the watchdog
		// Otherwise there is nothing to do in this loop
		usleep(10000);
	}
}

/*

Improvements:

- The delays have blocking behaviour, the CPU is not able to perform any other
  operations while it is waiting. A real-time operating system will solve this
  problem with its 'vTaskDelay()' function instead of 'sleep()' or 'usleep()'.

- There are two separate ISRs for the two buttons, they should be handled using
  one unified interrupt with an input argument for the identifiers of the
  buttons. Inside the ISR there should be a FIFO queue collecting the events.

*/
