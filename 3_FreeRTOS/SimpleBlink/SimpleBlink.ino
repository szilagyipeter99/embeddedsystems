#define LED1 25
#define LED2 26
#define LED3 17

void setup() {
    pinMode(LED3, OUTPUT);
  
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
void blink1(void *parameter) {
    pinMode(LED1, OUTPUT);
    while(1){
        digitalWrite(LED1, HIGH);
        delay(500); // Delay for Tasks 
        digitalWrite(LED1, LOW);
        delay(500);
    }
}
void blink2(void *parameter) {
    pinMode(LED2, OUTPUT);
    while(1) {
        digitalWrite(LED2, HIGH);
        delay(333);
        digitalWrite(LED2, LOW);
        delay(333);
    }
}
void loop(){
    digitalWrite(LED3, HIGH);
    delay(1111);
    digitalWrite(LED3, LOW);
    delay(1111);
}
