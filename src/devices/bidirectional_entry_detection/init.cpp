#include "bidirectional_entry_detection.h"

LiquidCrystal_I2C LCD_BIENTRY_DETECTION_SYSTEM(0x20, 16, 2);

void bidirectional_entry_detection_init() {
  LCD_BIENTRY_DETECTION_SYSTEM.init();
  LCD_BIENTRY_DETECTION_SYSTEM.backlight();
  pinMode(sensorA, INPUT);
  pinMode(sensorB, INPUT);
  pinMode(BUZZER_BidirectionalEntryDetection, OUTPUT);
  displayMessage(LCD_BIENTRY_DETECTION_SYSTEM, "No vistor", "Total: 0");
}