#include "driver/rtc_io.h"

#define LED_PIN 4

// Pins 0-7 on the ESP36-C6 are the only RTC IOs
#define BTN_1_PIN 6
#define BTN_2_PIN 7

bool sleepFlag = false;

// Bitwise left shift operation
// 1ULL is 1 as a 64-bit uint (unsigned long long)
// Shift the 1 by the amount of BTN_X_PIN (= 2 to the power of X)
// The variable in case of pins 6 and 7 is 64 + 128 = 192
uint64_t btns_1_2_bitmask = (1ULL << BTN_1_PIN) + (1ULL << BTN_2_PIN);

// Timer to send MCU to sleep
hw_timer_t *myTimer = NULL;

// Data to be retrieved after waking up
// Store variable in LP memory
RTC_DATA_ATTR uint8_t randomValue = 0;

// Handle timer ISR
void IRAM_ATTR onTimer() {
  sleepFlag = true;
}

void setup() {

  Serial.begin(115200);
  Serial.println("Starting...");

  pinMode(LED_PIN, OUTPUT);

  // Generate a random number if the variable is unknown
  // Will not change if it already has a value
  if (randomValue == 0) {
    randomValue = random(50, 200);
  }
  Serial.print("Random number: ");
  Serial.println(randomValue);

  // Setup RTC pins
  rtc_gpio_init((gpio_num_t)BTN_1_PIN);
  rtc_gpio_set_direction((gpio_num_t)BTN_1_PIN, RTC_GPIO_MODE_INPUT_ONLY);
  rtc_gpio_pulldown_dis((gpio_num_t)BTN_1_PIN);
  rtc_gpio_pullup_en((gpio_num_t)BTN_1_PIN);

  rtc_gpio_init((gpio_num_t)BTN_2_PIN);
  rtc_gpio_set_direction((gpio_num_t)BTN_2_PIN, RTC_GPIO_MODE_INPUT_ONLY);
  rtc_gpio_pulldown_dis((gpio_num_t)BTN_2_PIN);
  rtc_gpio_pullup_en((gpio_num_t)BTN_2_PIN);

  // Enable EXT1 wakeup to both buttons
  esp_sleep_enable_ext1_wakeup_io(btns_1_2_bitmask, ESP_EXT1_WAKEUP_ANY_LOW);

  // Setup a 6 second timer
  myTimer = timerBegin(1000000);
  timerAttachInterrupt(myTimer, &onTimer);
  timerAlarm(myTimer, 6000000, false, 0);

  xTaskCreate(blinkLED, "Blink LED", 2048, NULL, 1, NULL);
  xTaskCreate(handleSleep, "Handle Sleep", 2048, NULL, 1, NULL);
}

void blinkLED(void *params) {
  while (1) {
    analogWrite(LED_PIN, blinkBrightness);
    vTaskDelay(pdMS_TO_TICKS(randomValue));
    analogWrite(LED_PIN, 0);
    vTaskDelay(pdMS_TO_TICKS(randomValue));
  }
}

void handleSleep(void *params) {
  while (1) {
    if (sleepFlag == true) {
      // Not needed to put flag back to false because its value is lost
      Serial.println("I am going to sleep...");
      Serial.flush();
      esp_deep_sleep_start();
    }
    vTaskDelay(1);
  }
}

void loop() {}

// TODO: try to detect which button was used for wakeup
