#define DEVBOARD_LED_PIN 8
#define LED_BRIGHTNESS 64

void setup() {}

void loop() {
  blinkLED(500);
}

void blinkLED(int delayTime) {
  rgbLedWrite(DEVBOARD_LED_PIN, 0, LED_BRIGHTNESS, 0);
  delay(delayTime);
  rgbLedWrite(DEVBOARD_LED_PIN, LED_BRIGHTNESS, 0, 0);
  delay(delayTime);
}
