#pragma once
#include <Keypad.h>
#include <LiquidCrystal_I2C.h>
#include <ESP32Servo.h>
#include "utils/utils.h"
#include "../../connection/mqtt/mqtt.h"

const int BUZZER_DoorLockSystem = 1;
const int SERVO = 17;
const int BUTTON = 18;

extern LiquidCrystal_I2C LCD_DOOR_LOCK_SYSTEM;
extern Servo servo;
extern Keypad customKeypad;

extern bool isHomeEntryCompleted; // has the user entered the house and closed the door?
extern byte hashedPassword[32]; // 32 bytes for SHA-256 hash
extern String passwordPlaceholder; // for display on LCD screen

// Specification variables for websites inherited from `main.cpp`
extern bool isPasswordSet; // has the user set a password?
extern int tryAttempt;
extern int maxTryAttempt;
extern int passwordLength;
extern bool isMutedDoorSystem; // turn on/off sound effect
extern bool isDoorPermanentlyLocked; // type password wrong `maxTryAttempt` times, this will be true
extern bool isDoorLocked;
extern byte hashedPassword[32];

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

// Main program
void taskDoorLockSystem(void* parameter);