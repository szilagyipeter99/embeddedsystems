#include "temperature_monitor.h"

#define COLD_THRESHOLD 20.0
#define HUMIDITY_THRESHOLD 55.0

void setup() {

  Serial.begin(115200);
  Serial.println("Starting...");
  // Sizes in bytes (4 byte = 32 bit)
  Serial.println(sizeof(int));
  Serial.println(sizeof(int8_t));
  Serial.println(sizeof(float));
}

void loop() {

  TemperatureData data = readTemperature();

  int8_t roundedTemp = getRoundedTemperature(data);
  Serial.print("Temperature: ");
  Serial.print(roundedTemp);
  Serial.println(" °C");

  Serial.print("Humidity: ");
  Serial.print(data.humidity);
  Serial.println(" %");

  updateLED(data, COLD_THRESHOLD);

  if (isHumidityAboveThreshold(data, HUMIDITY_THRESHOLD)) {
    Serial.println("Humidity too high!");
  }

  Serial.println("---------------------------------");

  delay(2000);
}
