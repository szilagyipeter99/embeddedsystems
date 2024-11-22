#define LED_PIN 12
#define BTN_PIN 2

uint8_t ledState = 0;

void setup() {

  pinMode(LED_PIN, OUTPUT);
  pinMode(BTN_PIN, INPUT_PULLUP);

  digitalWrite(LED_PIN, 0);
}

void loop() {

  /*
  if (!digitalRead(BTN_PIN)) {
    ledState = ledState ^ 1;
    digitalWrite(LED_PIN, ledState);
  }
  */

  if (!digitalRead(BTN_PIN)) {
    delay(50);
    if (!digitalRead(BTN_PIN)) {
      ledState = ledState ^ 1;
      digitalWrite(LED_PIN, ledState);
      while (!digitalRead(BTN_PIN));
    }
  }

  delay(1);
}
