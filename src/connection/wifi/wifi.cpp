#include "wifi.h"

WiFiClient wifiClient;
const char* ssid = "Wokwi-GUEST";
const char* password = "";

void wifiSetup() {
    // Connect to WiFi
    WiFi.begin(ssid, password);

    // Wait for connection
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
    }
}