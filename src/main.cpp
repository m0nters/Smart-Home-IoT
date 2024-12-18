#include <Keypad.h>
#include <LiquidCrystal_I2C.h>
#include <ESP32Servo.h>
#include "devices/devices.h"
#include "connection/connection.h"

// Specification variables for websites
// for bidirectional entry detection
int i = 0;
int visitorCount = 0;
bool isMutedEntryDetection = false; // turn on/off sound effect

// for door lock system
bool isPasswordSet = false; // has the user set a password?
int tryAttempt = 0; // how many times user has typed password since last successful
int maxTryAttempt = 3; // how many times user can type password wrong before the door is permanently locked
int passwordLength = 4;
bool isMutedDoorSystem = false; // turn on/off sound effect
bool isDoorPermanentlyLocked = false; // type password wrong >= `maxTryAttempt` times, this will be true
bool isDoorLocked = true; // by default the door is locked, meaning even if the electricity goes off, the system is still safe
byte hashedPassword[32] = { 0 };

// light detection
int lightValueForChart = 0;
bool isAutomaticLight = 1;


// everything works in here, this is the entry point
void setup() {
  // first, initialize the network
  wifiSetup();
  mqttSetup();

  // then, initialize the devices
  door_locking_system_init();
  bidirectional_entry_detection_init();
  light_detection_init();
  fire_detection_init();

  // finally, start tasks
  xTaskCreate(taskLightDetection, "Light Detection", 4096, NULL, 2, NULL);
  xTaskCreate(taskFireDetection, "Fire Detection", 4096, NULL, 2, NULL);
  xTaskCreate(taskBidirectionalEntryDetection, "Bidirectional Entry Detection", 4096, NULL, 3, NULL);
  xTaskCreate(taskDoorLockSystem, "Door Lock System", 4096, NULL, 4, NULL);
  xTaskCreate(taskMQTT, "MQTT Task", 4096, NULL, 1, NULL);
}

void loop() {
  
}