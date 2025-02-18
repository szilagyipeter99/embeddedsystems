#include "temperature_monitor.h"

#define DEVBOARD_LED_PIN 8
#define LED_BRIGHTNESS 64

TemperatureData readTemperature() {
  TemperatureData data;
  data.temperature = random(15, 30) + 1.0 / random(1, 10);  // Wouldn't work with 1 / ...
  data.humidity = random(30, 70) + 1.0 / random(1, 10);
  return data;
}

void updateLED(TemperatureData data, float threshold) {

  if (data.temperature >= threshold) {
    rgbLedWrite(DEVBOARD_LED_PIN, 0, LED_BRIGHTNESS, 0);
    Serial.println("Normal temperature.");
  } else {
    rgbLedWrite(DEVBOARD_LED_PIN, LED_BRIGHTNESS, 0, 0);
    Serial.println("Low temperature.");
  }
}

bool isHumidityAboveThreshold(TemperatureData data, float threshold) {
  return data.humidity >= threshold;
}

int8_t getRoundedTemperature(TemperatureData data) {
  return static_cast<int8_t>(data.temperature + 0.5f);  // 0.5 works, but takes more bytes (double)
}
