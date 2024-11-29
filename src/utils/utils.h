#pragma once
#include <Arduino.h>
#include <LiquidCrystal_I2C.h>

extern LiquidCrystal_I2C LCD_DOOR_LOCK_SYSTEM;

// Function declarations
void displayMessage(LiquidCrystal_I2C& lcd, const String& line1, const String& line2);