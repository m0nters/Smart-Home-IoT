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
extern bool isMutedDoorSystem;
extern bool isDoorPermanentlyLocked;
extern int maxTryAttempt;
extern int passwordLength;

extern int lightValueForGauge;
extern float temperatureForGauge;
extern float humidityForGauge;
extern bool isAutomaticLight;
extern bool isFireAlarmSound;
extern bool isMistSpray;
extern int lightValueForGauge;
extern float temperatureForGauge;
extern float humidityForGauge;
extern bool isAutomaticLight;
extern bool isFireAlarmSound;
extern bool isMistSpray;

extern byte hashedPassword[32];

void mqttSetup();
void mqttConnect();
void websiteDataHandler(char* topic, byte* payload, unsigned int length);
void taskMQTT(void* parameter);