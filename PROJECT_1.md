# Project 1:

Connect a button and an LED to the development board. Build an ESP-IDF project that uses FreeRTOS to blink the LED and measure how long the button is held down. The LED should blink in a 375 ms interval. When the button is pressed, a measurement starts and the LED must stop blinking. Upon release, the latest measurement has to be saved as an integer and the LED should continue blinking. As previous measurement values are not needed, a single integer variable is sufficient.

Variable name | Pin number | Direction
--- | --- | ---
`READ_BTN` | 4 | Input
`LED` | 6 | Output

## UART and Deep sleep:

To retrieve the last measurement value and to send the MCU into deep sleep mode, UART messages are used. Measurements are saved in seconds, where results are not rounded, but floored. For example, more than 5 seconds but less than 6 seconds is saved as 5. In addition, the last measurement value should be available after waking up from deep sleep.

UART command | Description
--- | ---
`GET_VALUE` | Log the last measurement value
`GO_SLEEP` | Go to deep sleep

## FreeRTOS tasks:

Three tasks should be created according to the table below. Configurations for GPIOs or UART should be placed in the corresponding tasks, while the setup of EXT1 wakeup must reside in `app_main()`.

Task function | Description
--- | ---
`blink_led` | Blink the LED if there is no active measurement
`measure_btn` | Measure how long the button is held down
`handle_msg` | Read incoming UART messages and handle them
