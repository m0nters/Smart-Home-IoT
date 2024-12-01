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
int tryAttempt = 0;
const int maxTryAttempt = 3;
const int passwordLength = 4;
bool isMuted = false; // turn on/off sound effect

void setup() {
  door_locking_system_init();
  bidirectional_entry_detection_init();

  // Setup for Light Detection
  pinMode(LED_PIN, OUTPUT);
  pinMode(LDR_PIN, INPUT);

  // Set up for Fire Detection
  dhtSensor.setup(DHT_PIN, DHTesp::DHT22);
  pinMode(WATER_PIN, OUTPUT);
  pinMode(BUZZER_FIRE_PIN, OUTPUT);
  FastLED.addLeds<WS2812, NEO_LED_PIN, GRB>(leds, NUM_LEDS);
  FastLED.setBrightness(200); // Set brightness for the LED strip

  // Start tasks
  xTaskCreate(taskDoorLockSystem, "Door Lock System", 2048, NULL, 1, NULL);
  xTaskCreate(taskBidirectionalEntryDetection, "Bidirectional Entry Detection", 2048, NULL, 2, NULL);
  xTaskCreate(taskLightDetection, "Light Detection", 2048, NULL, 3, NULL);
  xTaskCreate(taskFireDetection, "Fire Detection", 2048, NULL, 4, NULL);
}

void loop() {
  // The loop is empty as tasks are being handled in the background.
}