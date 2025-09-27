// Include guard to prevent multiple inclusions
// More modern option: #pragma once
#ifndef TEMP_MONITOR_H
#define TEMP_MONITOR_H

// Include the following header to use fixed-width integer types like 'int8_t'
// (was automatically included in 'main.c')
#include <stdint.h>

float read_temperature();

int8_t get_rounded_temperature(float temperature);

void evaluate_temperature(float temperature, float threshold);

#endif
