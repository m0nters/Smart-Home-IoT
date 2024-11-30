#pragma once
#include "utils/utils.h"

extern int visitorCount;
extern const int BUZZER;
extern const int sensorA;
extern const int sensorB;
extern LiquidCrystal_I2C LCD_BIENTRY_DETECTION_SYSTEM;

// Function declarations
void bidirectional_entry_detection_init();
void handleVisitorArrival();
void handleVisitorExit();

// Main program
void taskBidirectionalEntryDetection(void* parameter);