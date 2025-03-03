#define LED1 25
#define LED2 26
TaskHandle_t taskBlink1Handle; // create task handle
TaskHandle_t taskBlink2Handle;
void setup() {
    Serial.begin(115200);
    pinMode(LED1, OUTPUT);
    pinMode(LED2, OUTPUT);
  
    xTaskCreate(
        blink1,      // Function name of the task
        "Blink 1",   // Name of the task (e.g. for debugging)
        2048,        // Stack size (bytes)
        NULL,        // Parameter to pass
        1,           // Task priority
        &taskBlink1Handle   // Assign task handle
      );
    xTaskCreate(
        blink2,     // Function name of the task
        "Blink 2",  // Name of the task (e.g. for debugging)
        1048,       // Stack size (bytes)
        NULL,       // Parameter to pass
        1,          // Task priority
        &taskBlink2Handle   // Assign task handle
    );
    xTaskCreate(
        printWatermark,     // Function name of the task
        "Print Watermark",  // Name of the task (e.g. for debugging)
        2048,        // Stack size (bytes)
        NULL,       // Parameter to pass
        1,          // Task priority
        NULL        // Task handle
    );
}
void blink1(void *pvParameters){
    while(1){
        digitalWrite(LED1, HIGH);
        delay(500); 
        digitalWrite(LED1, LOW);
        delay(500);
    }
}
void blink2(void *pvParameters){
    while(1){
        digitalWrite(LED2, HIGH);
        delay(333);
        digitalWrite(LED2, LOW);
        delay(333);
    }
}
void printWatermark(void *pvParameters){
    while(1){
        delay(2000);
        Serial.print("TASK: ");
        Serial.print(pcTaskGetName(taskBlink1Handle)); // Get task name with handler
        Serial.print(", High Watermark: ");
        Serial.print(uxTaskGetStackHighWaterMark(taskBlink1Handle));
        Serial.println();
        Serial.print("TASK: ");
        Serial.print(pcTaskGetName(taskBlink2Handle)); // Get task name with handler
        Serial.print(", High Watermark: ");
        Serial.print(uxTaskGetStackHighWaterMark(taskBlink2Handle));
        Serial.println();
    }
}
void loop(){}
