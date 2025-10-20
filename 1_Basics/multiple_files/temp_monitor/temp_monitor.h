// Include guard to prevent multiple inclusions
#ifndef TEMP_MONITOR_H
#define TEMP_MONITOR_H

// More modern option: #pragma once

float read_temperature();

int8_t get_rounded_temperature(float temperature);

void evaluate_temperature(float temperature, float threshold);

#endif
