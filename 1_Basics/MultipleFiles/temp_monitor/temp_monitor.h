// Include guard to prevent multiple inclusions
#ifndef TEMP_MONITOR_H
#define TEMP_MONITOR_H

#include <stdint.h> // To use fixed-width integer types like 'int8_t'

float readTemperature();

void evaluateTemperature(float temperature, float threshold);

int8_t getRoundedTemperature(float temperature);

#endif
