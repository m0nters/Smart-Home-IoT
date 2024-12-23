#pragma once
#include "../../connection/mqtt/mqtt.h"
#include <Arduino.h> 
#include "../../connection/mqtt/mqtt.h"

// Automatic light
const int LED_PIN = 12; 
const int LDR_PIN = 33;

extern int lightValueForGauge;

void taskLightDetection(void *parameter);
void light_detection_init();