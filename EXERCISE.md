# Exercise:

(Complete after __4_SleepModes__)

Connect a button and an LED to the development board. Build an ESP-IDF project using FreeRTOS that blinks the LED and measures how long the button is held down. The LED should blink with a 375 ms interval. When the button is pressed, the measurement starts and the LED must stop blinking. Upon release of the button, the latest measurement value has to be saved as an integer, and the LED should resume blinking. Since previous measurement values are not required, a single integer variable is sufficient.

Variable name | Pin number | Direction
--- | --- | ---
`READ_BTN` | 4 | Input
`LED` | 6 | Output

## UART and Deep sleep:

UART messages are used to retrieve the last measurement value and to put the MCU into deep sleep mode. Measurement values are stored in seconds using floor rounding. For example, a measurement longer than 5 seconds but shorter than 6 seconds is stored as 5. In addition, the last measurement value must remain available after waking up from deep sleep.

UART command | Description
--- | ---
`GET_VALUE` | Log the last measurement value
`GO_SLEEP` | Enter deep sleep mode

## FreeRTOS tasks:

Three FreeRTOS tasks must be created according to the table below. GPIO and UART configurations should be placed in the corresponding tasks, while the configuration of the EXT1 wakeup source must be implemented in `app_main()`.

Task function | Description
--- | ---
`blink_led` | Blink the LED when no measurement is active 
`measure_btn` | Measure how long the button is held down
`handle_msg` | Read and handle incoming UART messages
