#pragma once
#include <Arduino.h>
#include <LiquidCrystal_I2C.h>

extern LiquidCrystal_I2C lcd;

// Function declarations
void displayMessage(const String& line1, const String& line2);