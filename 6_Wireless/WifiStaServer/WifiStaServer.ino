#include <WiFi.h>
#include <WebServer.h>

const char *ssid = "EmbeddedAP";
const char *password = "EmbeddedAP123";

WebServer server(80);

void setup() {

    Serial.begin(115200);
    Serial.println("Starting...");

    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.print(".");
    }

    Serial.println();
    Serial.println("Connected to AP!");

    Serial.print("Client IP: ");
    Serial.println(WiFi.localIP());

    server.on("/", HTTP_GET, []() {
        server.send(200, "text/html", "<h1>ESP32C6 Server</h1><p>Hello World!</p>");
    });

    server.on("/status", HTTP_GET, []() {
        server.send(200, "application/json", "{\"status\":\"running\"}");
    });

    server.begin();
}

void loop() {
    server.handleClient();
}
