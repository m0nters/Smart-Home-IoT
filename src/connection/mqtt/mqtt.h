#pragma once
#include <PubSubClient.h>
#include "../wifi/wifi.h"

// MQTT broker details
extern const char* mqtt_server;
extern const int mqtt_port;

// MQTT client
extern PubSubClient mqttClient;

// variables from website to fetch
extern bool isMutedEntryDetection;

extern int lightValueForGauge;
extern bool isAutomaticLight;
extern bool isMistSpray;
extern byte hashedPassword[32];
extern float temperatureForGauge;
extern float humidityForGauge;
extern bool isFireAlarmSound;

void mqttSetup();
void mqttConnect();
void websiteDataHandler(char* topic, byte* payload, unsigned int length);
void taskMQTT(void* parameter);