#pragma once
#include <PubSubClient.h>
#include "../wifi/wifi.h"

// MQTT broker details
extern const char* mqtt_server;
extern const int mqtt_port;

// MQTT client
extern PubSubClient mqttClient;

// variables from website to fetch
extern bool isDoorPermanentlyLocked;
extern byte hashedPassword[32];
extern bool isMutedEntryDetection;
extern bool isMutedDoorSystem;

void mqttSetup();
void mqttConnect();
void websiteDataHandler(char* topic, byte* payload, unsigned int length);
void publishData();
void taskMQTT(void* parameter);