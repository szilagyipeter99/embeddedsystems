#ifndef TEMPERATURE_MONITOR_H
#define TEMPERATURE_MONITOR_H

#include <stdint.h>   // To use fixed-width integer types
#include <Arduino.h>  // To use rgbLedWritem(), Serial.println() ...

struct TemperatureData {
  float temperature;
  float humidity;
};

TemperatureData readTemperature();  // Returning a value of type TemperatureData

void updateLED(TemperatureData data);  // No return value

bool isHumidityAboveThreshold(TemperatureData data, float threshold);

int8_t getRoundedTemperature(TemperatureData data);
// Using 'int8_t' can save memory (8 bit instead of 32), but 'float' is 32 bit anyways
// So there isn't much point in trying to save space
// Could make code more portable across platforms
// -127 to 127

#endif
