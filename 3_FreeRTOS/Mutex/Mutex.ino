#define LED1 25
#define LED2 26
SemaphoreHandle_t mutex; // Create handle
void setup() {
    mutex = xSemaphoreCreateMutex(); //Create the mutex object
         
    xTaskCreate(
        blink1,      // Function name of the task
        "Blink 1",   // Name of the task (e.g. for debugging)
        2048,        // Stack size (bytes)
        NULL,        // Parameter to pass
        1,           // Task priority
        NULL         // Task handle
    );
    xTaskCreate(
        blink2,     // Function name of the task
        "Blink 2",  // Name of the task (e.g. for debugging)
        2048,       // Stack size (bytes)
        NULL,       // Parameter to pass
        1,          // Task priority
        NULL        // Task handle
    );
}
void blink1(void *pvParameters){
    pinMode(LED1, OUTPUT);
    while(1){
        xSemaphoreTake(mutex,portMAX_DELAY); // Take the mutex
        for(int i=0; i<10; i++){
            digitalWrite(LED1, HIGH);
            delay(250);  
            digitalWrite(LED1, LOW);
            delay(250); 
        }
        xSemaphoreGive(mutex); // Releases the mutex
        delay(200); // Short delay is needed!
    }
}
void blink2(void *pvParameters){
    pinMode(LED2, OUTPUT);
    while(1){
        xSemaphoreTake(mutex,portMAX_DELAY); // Take the mutex
        for(int i=0; i<10; i++){
            digitalWrite(LED2, HIGH);
            delay(333);
            digitalWrite(LED2, LOW);
            delay(333);   
        }
        xSemaphoreGive(mutex); // // Release the mutex
        delay(200); // Short delay is needed!
    }
}
void loop(){}
