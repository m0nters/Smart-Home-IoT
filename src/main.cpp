#include <Arduino.h> 
#include <Keypad.h>
#include <LiquidCrystal_I2C.h>
#include <ESP32Servo.h>
#include "door_lock_system/door_lock_system.h"
#include "bidirectional_entry_detection/bidirectional_entry_detection.h"
#include "light_detection/light_detection.h"
#include "fire_detection/fire_detection.h"
#include <PubSubClient.h>
#include <WiFi.h>

// Specification variables for websites
// for bidirectional entry detection
int visitorCount = 0;
bool isMutedEntryDetection = false; // turn on/off sound effect

// for door lock system
bool isPasswordSet = false; // has the user set a password?
int tryAttempt = 0;
int maxTryAttempt = 3;
int passwordLength = 4;
bool isMutedDoorSystem = false; // turn on/off sound effect
bool isDoorPermanentlyLocked = false; // type password wrong `maxTryAttempt` times, this will be true
bool isDoorLocked = false;
byte hashedPassword[32] = { 0 };


// WiFi credentials
const char* ssid = "Wokwi-GUEST";
const char* password = "";


// MQTT broker details
const char* mqtt_server = "test.mosquitto.org";
const int mqtt_port = 1883;


// MQTT client
WiFiClient wifiClient;
PubSubClient mqttClient(wifiClient);


// Function to connect to WiFi
void setup_wifi() {
  // Connect to WiFi
  WiFi.begin(ssid, password);

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
}


// Function to connect to MQTT broker
void reconnect() {
  while (!mqttClient.connected()) {
    if (mqttClient.connect("ESP32Client")) {
      // Subscribe to topics if needed
      mqttClient.subscribe("home-0PPKrXoRcgyppks/#");
    }
    else {
      delay(5000);
    }
  }
}


void callback(char* topic, byte* payload, unsigned int length) {
  String message;
  for (unsigned int i = 0; i < length; i++) {
    message += (char)payload[i];
  }

  if (String(topic) == "home-0PPKrXoRcgyppks/visitorCount") {
    visitorCount = message.toInt();
  }
  else if (String(topic) == "home-0PPKrXoRcgyppks/isPasswordSet") {
    isPasswordSet = (message == "true");
  }
  else if (String(topic) == "home-0PPKrXoRcgyppks/tryAttempt") {
    tryAttempt = message.toInt();
  }
  else if (String(topic) == "home-0PPKrXoRcgyppks/maxTryAttempt") {
    maxTryAttempt = message.toInt();
  }
  else if (String(topic) == "home-0PPKrXoRcgyppks/passwordLength") {
    passwordLength = message.toInt();
  }
  else if (String(topic) == "home-0PPKrXoRcgyppks/isMuted") {
    isMutedDoorSystem = (message == "true");
  }
  else if (String(topic) == "home-0PPKrXoRcgyppks/isDoorPermanentlyLocked") {
    isDoorPermanentlyLocked = (message == "true");
  }
}


void taskMQTT(void* parameter) {
  while (1) {
    if (WiFi.status() != WL_CONNECTED) {
      setup_wifi();  // Reconnect WiFi if disconnected
    }

    if (!mqttClient.connected()) {
      reconnect();
    }
    mqttClient.loop();
    vTaskDelay(10 / portTICK_PERIOD_MS); // Adjust delay as needed
  }
}


void publishData() {
  mqttClient.publish("home-0PPKrXoRcgyppks/visitorCount", String(visitorCount).c_str());
  mqttClient.publish("home-0PPKrXoRcgyppks/isMutedEntryDetection", isMutedEntryDetection ? "true" : "false");

  // Door lock system data
  mqttClient.publish("home-0PPKrXoRcgyppks/isPasswordSet", isPasswordSet ? "true" : "false");
  mqttClient.publish("home-0PPKrXoRcgyppks/tryAttempt", String(tryAttempt).c_str());
  mqttClient.publish("home-0PPKrXoRcgyppks/maxTryAttempt", String(maxTryAttempt).c_str());
  mqttClient.publish("home-0PPKrXoRcgyppks/passwordLength", String(passwordLength).c_str());
  mqttClient.publish("home-0PPKrXoRcgyppks/isMutedDoorSystem", isMutedDoorSystem ? "true" : "false");
  mqttClient.publish("home-0PPKrXoRcgyppks/isDoorPermanentlyLocked", isDoorPermanentlyLocked ? "true" : "false");
}


// everything works in here, this is the entry point
void setup() {
  setup_wifi();

  mqttClient.setServer(mqtt_server, mqtt_port);
  mqttClient.setCallback(callback);
  mqttClient.setKeepAlive(90);

  door_locking_system_init();
  bidirectional_entry_detection_init();
  light_detection_init();
  fire_detection_init();

  // Start tasks
  xTaskCreate(taskLightDetection, "Light Detection", 4096, NULL, 2, NULL);
  xTaskCreate(taskFireDetection, "Fire Detection", 4096, NULL, 2, NULL);
  xTaskCreate(taskBidirectionalEntryDetection, "Bidirectional Entry Detection", 4096, NULL, 3, NULL);
  xTaskCreate(taskDoorLockSystem, "Door Lock System", 4096, NULL, 4, NULL);
  xTaskCreate(taskMQTT, "MQTT Task", 4096, NULL, 1, NULL);

  // Publish data periodically
  xTaskCreate([](void* parameter) {
    while (true) {
      publishData();
      vTaskDelay(10 / portTICK_PERIOD_MS); // Publish data every 10 seconds
    }
    }, "Publish Data", 4096, NULL, 1, NULL);
}

void loop() {
  // The loop is empty as tasks are being handled in the background.
}