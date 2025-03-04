SemaphoreHandle_t myMutex;

void setup() {

  Serial.begin(115200);
  Serial.println("Starting...");

  myMutex = xSemaphoreCreateMutex();
  xTaskCreate(sendMsg1, "Send message #1", 2048, NULL, 1, NULL);
  xTaskCreate(sendMsg2, "Send message #2", 2048, NULL, 1, NULL);
}

void sendMsg1(void *param) {
  while (1) {
    xSemaphoreTake(myMutex, portMAX_DELAY);
    Serial.println("Task 1: Using Shared Resource");
    vTaskDelay(1000 / portTICK_PERIOD_MS);  // Simulating resource use
    xSemaphoreGive(myMutex);
    vTaskDelay(5 / portTICK_PERIOD_MS);
  }
}

void sendMsg2(void *param) {
  while (1) {
    xSemaphoreTake(myMutex, portMAX_DELAY);
    Serial.println("Task 2: Using Shared Resource");
    vTaskDelay(1000 / portTICK_PERIOD_MS);  // Simulating resource use
    xSemaphoreGive(myMutex);
    vTaskDelay(5 / portTICK_PERIOD_MS);
  }
}

void loop() {}
