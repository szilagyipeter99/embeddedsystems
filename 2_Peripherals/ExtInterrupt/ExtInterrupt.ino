#define LED_1_PIN 12
#define LED_2_PIN 13
#define BTN_1_PIN 18
#define BTN_2_PIN 19

// Size of 'bool' is 1 byte as a 8 bit int
// Better than 'int8_t' or 'uint8_t' for clarity
// Assigning 'false' or 'true' to an int would also work
volatile bool firstBlink = 0;
volatile bool secondBlink = 0;

// IRAM_ATTR flag:
// Tells the compiler and linker to place the ISR in RAM instead of flash memory
// This ensures faster access and reliability, prevents optimizations from the compiler
void IRAM_ATTR handleFirstISR() {
  firstBlink = 1;
}
void IRAM_ATTR handleSecondISR() {
  secondBlink = 1;
}
void setup() {

  pinMode(LED_1_PIN, OUTPUT);
  pinMode(LED_2_PIN, OUTPUT);

  pinMode(BTN_1_PIN, INPUT_PULLUP);
  pinMode(BTN_2_PIN, INPUT_PULLUP);

  attachInterrupt(BTN_1_PIN, handleFirstISR, FALLING);  // digitalPinToInterrupt() might be needed for other MCUs
  attachInterrupt(BTN_2_PIN, handleSecondISR, FALLING);
}

void loop() {

  // This is executed as soon as 'firstBlink' is True the CPU is available
  if (firstBlink) {
    digitalWrite(LED_1_PIN, 1);
    delay(500);
    digitalWrite(LED_1_PIN, 0);
    delay(500);
    digitalWrite(LED_1_PIN, 1);
    delay(500);
    digitalWrite(LED_1_PIN, 0);
    delay(500);
    digitalWrite(LED_1_PIN, 1);
    delay(500);
    digitalWrite(LED_1_PIN, 0);
    firstBlink = false;
  }

  if (secondBlink) {
    digitalWrite(LED_2_PIN, 1);
    delay(500);
    digitalWrite(LED_2_PIN, 0);
    delay(500);
    digitalWrite(LED_2_PIN, 1);
    delay(500);
    digitalWrite(LED_2_PIN, 0);
    delay(500);
    digitalWrite(LED_2_PIN, 1);
    delay(500);
    digitalWrite(LED_2_PIN, 0);
    secondBlink = false;
  }
}
