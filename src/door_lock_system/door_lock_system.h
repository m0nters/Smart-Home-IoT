#pragma once
#include <Keypad.h>
#include <LiquidCrystal_I2C.h>
#include <ESP32Servo.h>
#include "utils/utils.h"

// devices inherited from `main.cpp`
extern const int BUZZER;
extern const int SERVO;
extern const int BUTTON;
extern LiquidCrystal_I2C LCD_DOOR_LOCK_SYSTEM;
extern Servo servo;
extern Keypad customKeypad;

// global miscellaneous variables
extern bool isHomeEntryCompleted; // has the user entered the house and closed the door?
extern int tryAttempt;
extern const int maxTryAttempt;
extern const int passwordLength;
extern bool isMuted; // turn on/off sound effect

// Password setup
extern byte hashedPassword[32]; // 32 bytes for SHA-256 hash
extern String passwordPlaceholder; // for display on LCD screen

// Function declarations
void door_locking_system_init();
void inputPassword(String& buffer);
void setNewPassword();
void enterPassword();
bool validatePassword(const String& input);
void hashString(byte outputBuffer[32], const String& input);
void lockTheSystem();
void awaitDoorClosure();
void playTypingSound();
void playFailureSound();
void playSuccessSound();
void playDoorClosedSound();
void playWaitingToCloseDoorSound();
