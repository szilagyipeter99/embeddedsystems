#define LED_1_PIN 4
#define LED_2_PIN 5

void setup() {

  Serial.begin(115200);
  Serial.println("Starting...");
  Serial.println(portTICK_PERIOD_MS);
  Serial.println(configTICK_RATE_HZ);

  pinMode(LED_1_PIN, OUTPUT);
  pinMode(LED_2_PIN, OUTPUT);

  xTaskCreate(
    blinkLED1,   // Function name of the task, '&blinkLed1' would also work
    "Blink #1",  // Name of the task (e.g. for debugging)
    2048,        // Stack size (in bytes)
    NULL,        // Parameter to pass (NULL: a pointer to nowhere)
    1,           // Task priority
    NULL         // Task handle to interact with it
  );
  xTaskCreate(blinkLED2, "Blink #2", 2048, NULL, 1, NULL);
}

void blinkLED1(void *param) {
  while (1) {
    digitalWrite(LED_1_PIN, 1);
    vTaskDelay(500 / portTICK_PERIOD_MS);  // Same as 'delay(500);' on an ESP32
    digitalWrite(LED_1_PIN, 0);
    vTaskDelay(500 / portTICK_PERIOD_MS);
  }
}

void blinkLED2(void *param) {
  while (1) {
    digitalWrite(LED_2_PIN, 1);
    vTaskDelay(125 / portTICK_PERIOD_MS);
    digitalWrite(LED_2_PIN, 0);
    vTaskDelay(125 / portTICK_PERIOD_MS);
  }
}

void loop() {}
