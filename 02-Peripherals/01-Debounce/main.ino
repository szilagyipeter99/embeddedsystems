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

    delay(50);  // Blocking behaviour, FreeRTOS will solve this problem

    if (!digitalRead(BTN_PIN)) {
      ledState = ledState ^ 1;  // (^): Bitwise XOR operator to toggle state (If 0 Then 1, If 1 Then 0)
      digitalWrite(LED_PIN, ledState);
    }
  }

  delay(1);  // Helps the task scheduler by yielding control to other tasks
}

// Optional homework: Implement the distinction of a normal press, long press, double press
