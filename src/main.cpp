#include <Arduino.h> 
#include <Keypad.h>
#include <LiquidCrystal_I2C.h>
#include <ESP32Servo.h>
#include "door_lock_system/door_lock_system.h"
#include "bidirectional_entry_detection/bidirectional_entry_detection.h"
#include "light_detection/light_detection.h"
#include "fire_detection/fire_detection.h"

// Specification variables for websites
int visitorCount = 0;
bool isPasswordSet = false;
int tryAttempt = 0;
const int maxTryAttempt = 3;
const int passwordLength = 4;
bool isMuted = false; // turn on/off sound effect
bool isDoorPermanentlyLocked = false; // type password wrong `maxTryAttempt` times, this will be true

void setup() {
  door_locking_system_init();
  bidirectional_entry_detection_init();
  light_detection_init();
  fire_detection_init();

  // Start tasks
  xTaskCreate(taskLightDetection, "Light Detection", 2048, NULL, 1, NULL);
  xTaskCreate(taskFireDetection, "Fire Detection", 2048, NULL, 2, NULL);
  xTaskCreate(taskBidirectionalEntryDetection, "Bidirectional Entry Detection", 2048, NULL, 3, NULL);
  xTaskCreate(taskDoorLockSystem, "Door Lock System", 2048, NULL, 4, NULL);
}

void loop() {
  // The loop is empty as tasks are being handled in the background.
}