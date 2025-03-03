#define LED_1_PIN 4
#define LED_2_PIN 5
#define BTN_1_PIN 10
#define BTN_2_PIN 11

SemaphoreHandle_t mySem1;
SemaphoreHandle_t mySem2;

void IRAM_ATTR handleFirstISR() {
  xSemaphoreGiveFromISR(mySem1, NULL);
}
void IRAM_ATTR handleSecondISR() {
  xSemaphoreGiveFromISR(mySem2, NULL);
}
void setup() {

  pinMode(LED_1_PIN, OUTPUT);
  pinMode(BTN_1_PIN, INPUT_PULLUP);
  pinMode(LED_2_PIN, OUTPUT);
  pinMode(BTN_2_PIN, INPUT_PULLUP);

  mySem1 = xSemaphoreCreateBinary();
  mySem2 = xSemaphoreCreateBinary();
  xTaskCreate(blinkLED1, "Blink #1", 2048, NULL, 1, NULL);
  xTaskCreate(blinkLED2, "Blink #2", 2048, NULL, 1, NULL);
  // Wait for the semaphores to be created
  if (mySem1 != NULL) {
    attachInterrupt(BTN_1_PIN, handleFirstISR, FALLING);
  }
  if (mySem2 != NULL) {
    attachInterrupt(BTN_2_PIN, handleSecondISR, FALLING);
  }
}

void blinkLED1(void *param) {
  while (1) {
    if (xSemaphoreTake(mySem1, portMAX_DELAY)) {
      for (int i = 0; i < 10; i++) {
        digitalWrite(LED_1_PIN, 1);
        vTaskDelay(100 / portTICK_PERIOD_MS);
        digitalWrite(LED_1_PIN, 0);
        vTaskDelay(100 / portTICK_PERIOD_MS);
      }
    }
  }
}

void blinkLED2(void *param) {
  while (1) {
    if (xSemaphoreTake(mySem2, portMAX_DELAY)) {
      for (int i = 0; i < 10; i++) {
        digitalWrite(LED_2_PIN, 1);
        vTaskDelay(100 / portTICK_PERIOD_MS);
        digitalWrite(LED_2_PIN, 0);
        vTaskDelay(100 / portTICK_PERIOD_MS);
      }
    }
  }
}

void loop() {}
