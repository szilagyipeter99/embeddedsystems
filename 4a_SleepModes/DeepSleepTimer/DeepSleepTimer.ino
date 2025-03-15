#define LED_PIN 4

void setup() {

  Serial.begin(115200);
  Serial.println("Starting...");

  pinMode(LED_PIN, OUTPUT);

  // Wake up 5 seconds after a sleep is started
  esp_sleep_enable_timer_wakeup(5000000);  // uS

  xTaskCreate(blinkLED, "Blink LED", NULL, 1, NULL);
  xTaskCreate(handleSleep, "Handle Sleep", NULL, 1, NULL);
}

void blinkLED(void *param) {
  while (1) {
    digitalWrite(LED_PIN, 1);
    vTaskDelay(pdMS_TO_TICKS(125));
    digitalWrite(LED_PIN, 0);
    vTaskDelay(pdMS_TO_TICKS(125));
  }
}

void handleSleep(void *param) {
  while (1) {
    vTaskDelay(pdMS_TO_TICKS(4000));
    Serial.println("I am awake for now.");
    vTaskDelay(pdMS_TO_TICKS(2000));
    Serial.println("I am going to sleep...");
    // Ensure the UART buffer's contents have all been sent before the bus is shut down
    Serial.flush();
    // Start sleeping
    esp_deep_sleep_start();
    // This message will never be sent
    Serial.println("I have just woke up.");
  }
}

void loop() {}
