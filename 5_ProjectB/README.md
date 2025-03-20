# Project description:

Upon startup, generate a random number to determine the LED blink interval and start blinking immediately. After 6 seconds, reduce the LED brightness to 25% using PWM. After an additional 3 seconds, put the MCU into deep sleep. Configure EXT1 wakeup using buttons on pins 6 and 7. Upon wakeup, the LED should resume blinking with the same interval, ensuring the value is retained in low-power (LP) memory. FreeRTOS tasks and timer interrupts must be used to manage LED blinking, brightness adjustment, and sleep transitions.
