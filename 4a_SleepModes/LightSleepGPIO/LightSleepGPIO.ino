#define LED_PIN 4
#define BTN_PIN 10

// Empty array for incoming messages
#define MSG_BUFFER_SIZE 20
char msgBuffer[MSG_BUFFER_SIZE];
int buffPosIndex = 0;
// UART message to send the MCU to sleep
#define SLEEP_MSG "GO_SLEEP"

void setup() {

  Serial.begin(115200);
  Serial.println("Starting...");

  pinMode(LED_PIN, OUTPUT);
  pinMode(10, INPUT_PULLUP);

  // Wake up when the button's level is LOW
  gpio_wakeup_enable(gpio_num_t(BTN_PIN), GPIO_INTR_LOW_LEVEL);
  esp_sleep_enable_gpio_wakeup();

  xTaskCreate(blinkLED, "Blink LED", 2048, NULL, 1, NULL);
  xTaskCreate(handleSleep, "Handle Sleep", 2048, NULL, 1, NULL);
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
    while (Serial.available()) {
      char c = Serial.read();
      if (c == '\n') {
        msgBuffer[buffPosIndex] = '\0';
        buffPosIndex = 0;
        if (strcmp(msgBuffer, SLEEP_MSG) == 0) {
          esp_light_sleep_start();
        }
      } else if (buffPosIndex < MSG_BUFFER_SIZE - 1) {
        msgBuffer[buffPosIndex++] = c;
      }
    }
  }
}

void loop() {}
