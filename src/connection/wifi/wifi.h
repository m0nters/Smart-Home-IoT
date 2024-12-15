#pragma once
#include <WiFi.h>

extern WiFiClient wifiClient;
// WiFi credentials
extern const char* ssid;
extern const char* password;

void wifiSetup();