#include <Arduino.h> 
#include <Keypad.h>
#include <LiquidCrystal_I2C.h>
#include <ESP32Servo.h>
#include "door_lock_system/door_lock_system.h"
#include "bidirectional_entry_detection/bidirectional_entry_detection.h"

// pin setup
const int BUZZER = 5;
const int SERVO = 17;
const int BUTTON = 18;
const int sensorA = 23;
const int sensorB = 19;

// LCD setup
LiquidCrystal_I2C LCD_DOOR_LOCK_SYSTEM(0x27, 16, 2);
LiquidCrystal_I2C LCD_BIENTRY_DETECTION_SYSTEM(0x20, 16, 2);

// Servo setup
Servo servo;

// Keypad setup
const byte ROWS = 4;
const byte COLS = 4;
char hexaKeys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
byte rowPins[ROWS] = { 16, 4, 0, 2 };
byte colPins[COLS] = { 15, 8, 7, 6 };
Keypad customKeypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);

// global miscellaneous variables

// for bidirectional entry detection system
int visitorCount = 0;
int entranceScanInterval = 5000; // delay too slow may cause the detection to detect the same visitor multiple times
// for door locking system
bool isHomeEntryCompleted = false; // has the user entered the house and closed the door?
int tryAttempt = 0;
const int maxTryAttempt = 3;
const int passwordLength = 4;
bool isMuted = false; // turn on/off sound effect

// Password setup
byte hashedPassword[32]; // 32 bytes for SHA-256 hash
String passwordPlaceholder = ""; // for display on LCD screen


void setup() {
  LCD_DOOR_LOCK_SYSTEM.init();
  LCD_DOOR_LOCK_SYSTEM.backlight();
  LCD_BIENTRY_DETECTION_SYSTEM.init();
  LCD_BIENTRY_DETECTION_SYSTEM.backlight();
  servo.attach(SERVO); // setup servo, for locking/unlocking the door
  pinMode(BUTTON, INPUT); // for sending a signal to lock the door again when enter the house
  pinMode(BUZZER, OUTPUT); // for auditory notification
  door_locking_system_init();
  bidirectional_entry_detection_init();
}


int mode = 0;
void loop() {
  if (mode == 0) {
    // Door closed as default in the beginning
    servo.write(0); // If not set, at default, its angle is 90°!

    // Display welcome message
    displayMessage(LCD_DOOR_LOCK_SYSTEM, "Welcome home!", "");
    delay(2000);

    // Check if the password hash has been initialized
    bool isPasswordSet = false;
    for (int i = 0; i < 32; i++) {
      if (hashedPassword[i] != 0) {
        isPasswordSet = true;
        break;
      }
    }

    if (!isPasswordSet) {
      // If not, set new password
      setNewPassword();
    }

    // Otherwise, just enter the existing password to unlock the door
    enterPassword();
  }
  if (mode == 1) {
    if (digitalRead(sensorA) == HIGH) {
      while (true) { // wait for the sensor b to detect
        if (digitalRead(sensorB) == HIGH) {
          handleVisitorArrival();
          delay(entranceScanInterval);
          visitorCount ? displayMessage(LCD_BIENTRY_DETECTION_SYSTEM, "No movement", "Total: " + String(visitorCount)) : displayMessage(LCD_BIENTRY_DETECTION_SYSTEM, "No vistors", "Total: 0");
          return;
        }
      }
    }
    if (digitalRead(sensorB) == HIGH) {
      while (true) { // wait for the sensor a to detect
        if (digitalRead(sensorA) == HIGH) {
          handleVisitorExit();
          delay(entranceScanInterval);
          visitorCount ? displayMessage(LCD_BIENTRY_DETECTION_SYSTEM, "No movement", "Total: " + String(visitorCount)) : displayMessage(LCD_BIENTRY_DETECTION_SYSTEM, "No vistors", "Total: 0");
          return;
        }
      }
    }
  }
}