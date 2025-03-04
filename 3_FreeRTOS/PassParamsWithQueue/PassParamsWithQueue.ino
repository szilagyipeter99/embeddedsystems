QueueHandle_t myQueue;  // Queue handle

void setup() {
  
  Serial.begin(115200);

  myQueue = xQueueCreate(5, sizeof(int));  // Can hold 5 integers (with default int size)
  xTaskCreate(sendMsg, "Send message", 2048, NULL, 1, NULL);
  xTaskCreate(receiveMsg, "Receive message", 2048, NULL, 1, NULL);
}

void sendMsg(void *param) {
  int cntr = 0;
  while (1) {
    cntr++;
    // Wait for 0 ticks, then try to get a place in the queue
    if (xQueueSend(myQueue, &cntr, 0) == pdTRUE) {
      Serial.print("Sent: ");
      Serial.println(cntr);
    }
    // New data could not be added to the queue
    else {
      Serial.print("Queue full, ");
      Serial.print(cntr);
      Serial.println(" is not in the queue!");
    }
    vTaskDelay(1000 / portTICK_PERIOD_MS);  // Try sending every 1s
  }
}

void receiveMsg(void *param) {
  int receivedValue;
  while (1) {
    // Take an element from the queue
    xQueueReceive(myQueue, &receivedValue, portMAX_DELAY);
    Serial.print("Received: ");
    Serial.println(receivedValue);
    vTaskDelay(2250 / portTICK_PERIOD_MS);  // Simulate long processing time
  }
}

void loop() {}
