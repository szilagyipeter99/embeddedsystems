#include <WiFi.h>
#include <HTTPClient.h>

const char *ssid = "EmbeddedAP";
const char *password = "EmbeddedAP123";
const char *serverUrl = "http://192.168.2.X";

void setup() {

  Serial.begin(115200);
  Serial.println("Starting...");

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }

  Serial.println();
  Serial.println("Connected to Server!");

  Serial.print("IP: ");
  Serial.println(WiFi.localIP());

  fetchData();
}

void loop() {
  delay(5000);
  fetchData();
}

void fetchData() {

  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    http.begin(String(serverUrl) + "/status");

    int httpResponseCode = http.GET();

    if (httpResponseCode > 0) {
      Serial.print("Response Code: ");
      Serial.println(httpResponseCode);
      String response = http.getString();
      Serial.println("Response: " + response);
    } else {
      Serial.print("Error on HTTP request: ");
      Serial.println(httpResponseCode);
    }

    http.end();
  } else {
    Serial.println("WiFi not connected!");
  }
}
