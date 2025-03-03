#define LED1 25
#define LED2 26
#define INT_PIN_1 15
#define INT_PIN_2 16
SemaphoreHandle_t interruptSemaphore1; // Create semaphore handle
SemaphoreHandle_t interruptSemaphore2;
void IRAM_ATTR keyISR1() {   // ISR definition
    xSemaphoreGiveFromISR(interruptSemaphore1, NULL);
}
void IRAM_ATTR keyISR2() {
    xSemaphoreGiveFromISR(interruptSemaphore2, NULL);
}
void setup() {
    interruptSemaphore1 = xSemaphoreCreateBinary(); // Create semaphore
    interruptSemaphore2 = xSemaphoreCreateBinary();
    xTaskCreate(
        blink1,      // Function name of the task
        "Blink 1",   // Name of the task (e.g. for debugging)
        2048,        // Stack size (bytes)
        NULL,        // Parameter to pass
        1,           // Task priority
        NULL         // Task handle
    );
    xTaskCreate(
        blink2,      // Function name of the task
        "Blink 2",   // Name of the task (e.g. for debugging)
        2048,        // Stack size (bytes)
        NULL,        // Parameter to pass
        1,           // Task priority
        NULL         // Task handle
    );
    if (interruptSemaphore1 != NULL) {
        attachInterrupt(digitalPinToInterrupt(INT_PIN_1), keyISR1, FALLING);
    }
    if (interruptSemaphore2 != NULL) {
        attachInterrupt(digitalPinToInterrupt(INT_PIN_2), keyISR2, FALLING);
    }
}
void blink1(void *pvParameters){
    pinMode(LED1, OUTPUT);
    pinMode(INT_PIN_1, INPUT_PULLUP);
    while(1){
        if (xSemaphoreTake(interruptSemaphore1, portMAX_DELAY)) {
            for(int i=0; i<10; i++){
                digitalWrite(LED1, HIGH);
                delay(50); // Delay for Tasks 
                digitalWrite(LED1, LOW);
                delay(50);
            }
        }
    }
}
void blink2(void *pvParameters){
    pinMode(LED2, OUTPUT);
    pinMode(INT_PIN_2, INPUT_PULLUP);
    while(1){
        if (xSemaphoreTake(interruptSemaphore2, portMAX_DELAY)) {
            for(int i=0; i<10; i++){
                digitalWrite(LED2, HIGH);
                delay(50);  
                digitalWrite(LED2, LOW);
                delay(50);
            }
        }
    }
}
void loop(){}
