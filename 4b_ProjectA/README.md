# Project description:

Blink an LED at a frequency of 5 Hz indefinitely while continuously listening for incoming serial messages. Upon receiving the message "GO_SLEEP", put the MCU into light sleep mode. Configure a GPIO pin as a wakeup source to wake the MCU upon a button press. On the third wakeup event, start blinking a second LED at a frequency of 8 Hz indefinitely. The usage of FreeRTOS tasks is required.
