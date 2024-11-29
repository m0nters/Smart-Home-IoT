#include "bidirectional_entry_detection.h"

void bidirectional_entry_detection_init() {
  displayMessage(LCD_BIENTRY_DETECTION_SYSTEM, "No vistor", "Total: 0");
  // Configure PIR sensor pins
  pinMode(sensorA, INPUT);
  pinMode(sensorB, INPUT);
}