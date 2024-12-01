#ifndef FIRE_DETECTION_H
#define FIRE_DETECTION_H
#pragma once

#include <Arduino.h> 
#include <DHTesp.h>
#include <FastLED.h>
// Fire detection pin
const int DHT_PIN = 14;
const int NEO_LED_PIN = 27;
const int NUM_LEDS = 12;
const int WATER_PIN = 26;
const int BUZZER_FIRE_PIN = 25;

extern CRGB leds[NUM_LEDS];    // Khai báo biến LEDs
extern DHTesp dhtSensor;       // Khai báo biến DHT


void taskFireDetection(void * parameter);
void fire_detection_init();

#endif

