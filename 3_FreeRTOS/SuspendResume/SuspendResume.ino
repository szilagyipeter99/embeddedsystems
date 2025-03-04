#define LED_PIN 4

TaskHandle_t blinkLEDHandle;

void setup() {

  pinMode(LED_PIN, OUTPUT);

  xTaskCreate(blinkLED, "Blink", 2048, NULL, 1, &blinkLEDHandle);
  xTaskCreate(suspendResume, "SuspendAndResume", 2048, NULL, 1, NULL);
}

void blinkLED(void *param) {
  while (1) {
    digitalWrite(LED_PIN, 1);
    vTaskDelay(125 / portTICK_PERIOD_MS);
    digitalWrite(LED_PIN, 0);
    vTaskDelay(125 / portTICK_PERIOD_MS);
  }
}

void suspendResume(void *param) {
  while (1) {
    vTaskDelay(3000 / portTICK_PERIOD_MS);
    vTaskSuspend(blinkLEDHandle);
    vTaskDelay(1000 / portTICK_PERIOD_MS);
    vTaskResume(blinkLEDHandle);
  }
}

void loop() {}
