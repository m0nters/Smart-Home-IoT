#ifndef LIGHT_DETECTION_H
#define LIGHT_DETECTION_H
#pragma once
#include "../../connection/mqtt/mqtt.h"
#include <Arduino.h> 

// Automatic light
const int LED_PIN = 12; 
const int LDR_PIN = 33;

extern int lightValueForGauge;

void taskLightDetection(void *parameter);
void light_detection_init();

#endif