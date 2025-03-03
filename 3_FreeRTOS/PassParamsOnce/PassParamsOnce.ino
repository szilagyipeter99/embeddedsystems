#define LED1 25
#define LED2 26
struct genericBlink{
    int pin;
    int period;
};
genericBlink ledBlink1 = {LED1, 500};
genericBlink ledBlink2 = {LED2, 333};
void setup() {
    xTaskCreate(
        &blink,      // Function name of the task
        "Blink 1",   // Name of the task (e.g. for debugging)
        2048,        // Stack size (bytes)
        (void*) &ledBlink1,  // Parameter to pass
        1,           // Task priority
        NULL         // Task handle
    );
    xTaskCreate(
        &blink,      // Function name of the task
        "Blink 2",   // Name of the task (e.g. for debugging)
        2048,        // Stack size (bytes)
        (void*) &ledBlink2,  // Parameter to pass
        1,           // Task priority
        NULL         // Task handle
    );
}
void blink(void *ledx){
    genericBlink *ledBlink = (genericBlink *) ledx;
    pinMode(ledBlink->pin, OUTPUT);
    while(1){
        digitalWrite(ledBlink->pin, HIGH);
        delay(ledBlink->period);
        digitalWrite(ledBlink->pin, LOW);
        delay(ledBlink->period);
    }
}
// void blink(void *ledx){
//     genericBlink ledBlink = *(genericBlink *) ledx;
//     pinMode(ledBlink.pin, OUTPUT);
//     while(1){
//         digitalWrite(ledBlink.pin, HIGH);
//         delay(ledBlink.period);
//         digitalWrite(ledBlink.pin, LOW);
//         delay(ledBlink.period);
//     }
// }
void loop(){}
