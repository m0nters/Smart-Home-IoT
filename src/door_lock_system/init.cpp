#include "door_lock_system.h"

// LCD setup
LiquidCrystal_I2C LCD_DOOR_LOCK_SYSTEM(0x27, 16, 2);

// Servo setup
Servo servo;

// Keypad setup
const int ROWS = 4;
const int COLS = 4;
char hexaKeys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
byte rowPins[ROWS] = { 16, 4, 0, 2 };
byte colPins[COLS] = { 15, 8, 7, 6 };
Keypad customKeypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);

// password setup
bool isHomeEntryCompleted = false;
byte hashedPassword[32] = { 0 };
String passwordPlaceholder = "";

void door_locking_system_init() {
  LCD_DOOR_LOCK_SYSTEM.init();
  LCD_DOOR_LOCK_SYSTEM.backlight();
  servo.attach(SERVO);
  pinMode(BUTTON, INPUT);
  pinMode(BUZZER_DoorLockSystem, OUTPUT);

  displayMessage(LCD_DOOR_LOCK_SYSTEM, "Welcome home!", "");
  passwordPlaceholder += "[";
  for (int i = 0; i < passwordLength; i++) {
    passwordPlaceholder += "_";
  }
  passwordPlaceholder += "]";
}