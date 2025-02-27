#define LED_PIN 12
#define BTN_PIN 2 // Other end to GND

uint8_t ledState = 0;
uint8_t btnState = 1;
uint8_t prevBtnState = 1;

void setup() {

  pinMode(LED_PIN, OUTPUT);
  pinMode(BTN_PIN, INPUT_PULLUP);

  digitalWrite(LED_PIN, 0);
}

void loop() {

  btnState = digitalRead(BTN_PIN);

  // if (btnState != prevBtnState && btnState == 0) {
  //   ledState = ledState ^ 1;
  //   digitalWrite(LED_PIN, ledState);
  // }

  if (btnState != prevBtnState) {  // '&& btnState == 0' is not here, so bouncing is resolved on the release, too

    delay(5);  // Blocking behaviour, FreeRTOS will solve this problem

    btnState = digitalRead(BTN_PIN);

    if (btnState != prevBtnState && btnState == 0) {
      ledState = ledState ^ 1;  // (^): Bitwise XOR operator to toggle state (If 0 Then 1, If 1 Then 0)
      digitalWrite(LED_PIN, ledState);
    }
  }
  prevBtnState = btnState;

}

// TODO: Implement the handling of a normal press, long press, double press
