#pragma once
#include "utils/utils.h"

extern int visitorCount;
extern const int BUZZER;
extern const int sensorA;
extern const int sensorB;

// Function declarations
void bidirectional_entry_detection_init();
void handleVisitorArrival();
void handleVisitorExit();