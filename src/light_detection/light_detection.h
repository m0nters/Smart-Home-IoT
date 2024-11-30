#ifndef LIGHT_DETECTION_H
#define LIGHT_DETECTION_H
#pragma once

#include <Arduino.h> 

// Automatic light
const int LED_PIN = 12; 
const int LDR_PIN = 13;

void taskLightDetection(void *parameter);

#endif