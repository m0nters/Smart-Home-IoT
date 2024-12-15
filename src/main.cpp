#include <Keypad.h>
#include <LiquidCrystal_I2C.h>
#include <ESP32Servo.h>
#include "./devices/devices.h"
#include "connection/wifi/wifi.h"
#include "connection/mqtt/mqtt.h"

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
bool isDoorPermanentlyLocked = false; // type password wrong maxTryAttempt times, this will be true
bool isDoorLocked = true; // by default the door is locked, meaning even if the electricity goes off, the system is still safe
byte hashedPassword[32] = { 0 };
















// everything works in here, this is the entry point
void setup() {
  wifiSetup();
  mqttSetup();

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
}

void loop() {

}