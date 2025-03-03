#define LED1 25
TaskHandle_t blink1Handle;
void setup() {
    xTaskCreate(
        blink1,       // Function name of the task
        "Blink 1",    // Name of the task (e.g. for debugging)
        2048,         // Stack size (bytes)
        NULL,         // Parameter to pass
        1,            // Task priority
        &blink1Handle // Task handle
    );
    xTaskCreate(
        suspend_resume,       // Function name of the task
        "Suspend Resume",    // Name of the task (e.g. for debugging)
        2048,         // Stack size (bytes)
        NULL,         // Parameter to pass
        1,            // Task priority
        NULL // Task handle
    );
}
void blink1(void *pvParameters){
    pinMode(LED1, OUTPUT);
    while(1){
        digitalWrite(LED1, HIGH);
        delay(100); 
        digitalWrite(LED1, LOW);
        delay(100);
    }
}
void suspend_resume(void *pvParameters){
    pinMode(LED1, OUTPUT);
    while(1){
        delay(1999);
        vTaskSuspend(blink1Handle);
        delay(999);
        vTaskResume(blink1Handle);
    }
}
void loop(){}
