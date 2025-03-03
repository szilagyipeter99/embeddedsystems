#define LED_1_PIN 4
#define LED_2_PIN 5

SemaphoreHandle_t mySem;  // Create semaphore handle

void setup() {

  pinMode(LED_1_PIN, OUTPUT);
  pinMode(LED_2_PIN, OUTPUT);

  mySem = xSemaphoreCreateBinary();
  xTaskCreate(blinkLED1, "Blink #1", 2048, NULL, 1, NULL);
  xTaskCreate(blinkLED2, "Blink #2", 2048, NULL, 1, NULL);
}

void blinkLED1(void *param) {
  while (1) {
    for (int i = 0; i < 5; i++) {
      digitalWrite(LED_1_PIN, 1);
      vTaskDelay(250 / portTICK_PERIOD_MS);
      digitalWrite(LED_1_PIN, 0);
      vTaskDelay(250 / portTICK_PERIOD_MS);
    }
    xSemaphoreGive(mySem);  // Release semaphore
    for (int i = 0; i < 5; i++) {
      digitalWrite(LED_1_PIN, 1);
      vTaskDelay(125 / portTICK_PERIOD_MS);
      digitalWrite(LED_1_PIN, 0);
      vTaskDelay(125 / portTICK_PERIOD_MS);
    }
    vTaskDelay(2000 / portTICK_PERIOD_MS);
  }
}

void blinkLED2(void *param) {
  while (1) {
    xSemaphoreTake(mySem, portMAX_DELAY);
    for (int i = 0; i < 10; i++) {
      digitalWrite(LED_2_PIN, 1);
      vTaskDelay(50 / portTICK_PERIOD_MS);
      digitalWrite(LED_2_PIN, 0);
      vTaskDelay(50 / portTICK_PERIOD_MS);
    }
  }
}

void loop() {}
