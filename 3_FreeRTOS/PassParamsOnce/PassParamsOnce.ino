#define LED_1_PIN 4
#define LED_2_PIN 5

struct blinkStructure {
  int pin;
  int interval;
};

blinkStructure blinkLED1 = { LED_1_PIN, 425 };
blinkStructure blinkLED2 = { LED_2_PIN, 775 };

void setup() {
  // xTaskCreate() expects the parameter to be of type 'void *'
  xTaskCreate(&setupBlink, "Blink #1", 2048, (void *)&blinkLED1, 1, NULL);
  xTaskCreate(&setupBlink, "Blink #2", 2048, (void *)&blinkLED2, 1, NULL);
}

void setupBlink(void *someBlinkStruct) {
  // Cast back the input parameter from (void *) to (blinkStructure *)
  blinkStructure *myBlink = (blinkStructure *)someBlinkStruct;
  pinMode(myBlink->pin, OUTPUT);
  while (1) {
    digitalWrite(myBlink->pin, 1);  // Would be 'myBlink.pin' if it was not a pointer
    vTaskDelay(myBlink->interval / portTICK_PERIOD_MS);
    digitalWrite(myBlink->pin, 0);
    vTaskDelay(myBlink->interval / portTICK_PERIOD_MS);
  }
}

void loop() {}
