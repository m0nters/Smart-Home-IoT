#pragma once
#include "utils/utils.h"
#include "../../connection/mqtt/mqtt.h"

const int sensorA = 23;
const int sensorB = 19;
const int BUZZER_BidirectionalEntryDetection = 5;
extern LiquidCrystal_I2C LCD_BIENTRY_DETECTION_SYSTEM;

// Specification variables for websites inherited from `main.cpp`
extern int visitorCount;
extern bool isMutedEntryDetection; // turn on/off sound effect

// Function declarations
void bidirectional_entry_detection_init();
void handleVisitorArrival();
void handleVisitorExit();

// Main program
void taskBidirectionalEntryDetection(void* parameter);