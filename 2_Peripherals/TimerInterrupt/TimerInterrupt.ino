// Timer structure
hw_timer_t *myFirstTimer = NULL;

volatile bool checkTempFlag = false;  // could be '= 0'

// Timer ISR
void IRAM_ATTR onTempTimer() {
  checkTempFlag = true;  // could be '= 1'
}

// Simulate temperature checking
void checkTemp() {
  uint8_t randomTemp = random(20, 40);
  Serial.print("Temperature: ");
  Serial.print(randomTemp);
  Serial.println(" °C");
}

void setup() {

  Serial.begin(115200);

  // Initialize hardware timer
  // The driver will choose an available hardware timer by itself
  myFirstTimer = timerBegin(1000000);  // 1 Mhz frequency, 1'000'000 ticks / second
  timerAttachInterrupt(myFirstTimer, &onTempTimer);
  timerAlarm(myFirstTimer, 1000000, true, 0);  // Repeat the alarm every 1'000'000 ticks indefinitely
}

void loop() {

  if (checkTempFlag) {
    checkTempFlag = false;
    checkTemp();
  }

  // Any other task
}
