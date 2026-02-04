### About the example:

This example demonstrates the use of the manufacturer-provided embedded driver for the Sensirion SHT40, SHT41, and SHT45 temperature and humidity sensors with ESP-IDF.

Original repository:
https://github.com/Sensirion/embedded-i2c-sht4x

As stated by Sensirion, the  `sensirion_i2c_hal.c` file (Hardware Abstraction Layer) must be adapted to the target hardware platform. All other source files contain generic C code used to communicate with the sensor and do not require any modifications.
