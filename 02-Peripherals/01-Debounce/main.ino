#define LED    12
#define BUTTON 2

uint8_t stateLED = 0;

void setup() {
    pinMode(LED, OUTPUT);
    pinMode(BUTTON,INPUT_PULLUP);
}

void loop() {

   if(!digitalRead(BUTTON)){
     stateLED = stateLED^1;
    digitalWrite(LED,stateLED);
  }
}
