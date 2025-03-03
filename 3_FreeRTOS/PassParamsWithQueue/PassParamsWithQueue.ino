QueueHandle_t queue1; // Create handle
void setup() {
    Serial.begin(115200);
    queue1 = xQueueCreate(1, sizeof(unsigned long));  // Create queue
  
    xTaskCreate(
        measureTime,    // Function name of the task
        "Measure Time", // Name of the task (e.g. for debugging)
        2048,           // Stack size (bytes)
        NULL,           // Parameter to pass
        1,              // Task priority
        NULL            // Task handle
    );
    xTaskCreate(
        printTime,    // Function name of the task
        "Print time", // Name of the task (e.g. for debugging)
        2048,         // Stack size (bytes)
        NULL,         // Parameter to pass
        1,            // Task priority
        NULL          // Task handle
    );
}
void measureTime(void *pvParameters){
    while(1){
        static unsigned long startTime = millis();
        unsigned long timeSinceStart = (millis() - startTime)/1000;
        xQueueSend(queue1, &timeSinceStart, 0);  // Send queue
        delay(100);
    }
}
void printTime(void *pvParameters){
    while(1){
        unsigned long buf;
        xQueueReceive(queue1, &buf, 0); // Receive queue
        Serial.print("Time since start: ");
        Serial.println(buf);
        delay(2000);
    }
}
void loop(){}
