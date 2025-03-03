#define LED1 25
#define LED2 26
SemaphoreHandle_t countingSem; // Create handle 
SemaphoreHandle_t countingSem2;
void setup() {
    Serial.begin(115200);
    pinMode(LED1, OUTPUT);
    pinMode(LED2, OUTPUT);
    countingSem = xSemaphoreCreateCounting(1,1); // Create counting semaphore
    countingSem2 = xSemaphoreCreateCounting(4,4); // Create second counting semaphore  
    Serial.print("countingSem2 left: ");
    Serial.println(uxSemaphoreGetCount(countingSem2));
    
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
    while(1){
        /* Take the semaphore, no semaphore (countingSem) will be left */
        xSemaphoreTake(countingSem, portMAX_DELAY);
        /* Take the semaphore countingSem2 if still available */
        xSemaphoreTake(countingSem2, portMAX_DELAY);
        Serial.print("countingSem2 left: ");
        Serial.println(uxSemaphoreGetCount(countingSem2));
        
        for(int i=0; i<10; i++){
            digitalWrite(LED1, HIGH);
            delay(250);
            digitalWrite(LED1, LOW);
            delay(250); 
        }
        /* Give only semaphore countingSem */
        xSemaphoreGive(countingSem);
        delay(200); // Short delay is needed!
    }
}
void blink2(void *pvParameters){
    while(1){
        /* Take the semaphore, no semaphore (countingSem) will be left */
        xSemaphoreTake(countingSem, portMAX_DELAY);
        /* Take the semaphore countingSem2 if still available */
        xSemaphoreTake(countingSem2, portMAX_DELAY);
        Serial.print("countingSem2 left: ");
        Serial.println(uxSemaphoreGetCount(countingSem2));
        for(int i=0; i<10; i++){
            digitalWrite(LED2, HIGH);
            delay(333);
            digitalWrite(LED2, LOW);
            delay(333);   
        }
        /* Give only semaphore countingSem */
        xSemaphoreGive(countingSem);
        delay(200);  // Short delay is needed!
    }
}
void loop(){}
