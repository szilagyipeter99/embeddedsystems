#include <WiFi.h>

const char *ssid = "EmbeddedAP";
const char *password = "EmbeddedAP123";

const int maxClients = 8;

void setup() {
  Serial.begin(115200);
  Serial.println("Starting...");

  bool success = WiFi.softAP(ssid, password, 1, 0, maxClients);

  if (success) {
    Serial.println("SoftAP started successfully!");
    Serial.print("SSID: ");
    Serial.println(ssid);
    Serial.print("IP Address: ");
    Serial.println(WiFi.softAPIP());
  } else {
    Serial.println("SoftAP failed to start!");
  }
}

void loop() {}
