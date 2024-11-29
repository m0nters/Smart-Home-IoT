#include "bidirectional_entry_detection.h"

void bidirectional_entry_detection_init() {
  // Configure PIR sensor pins
  pinMode(sensorA, INPUT);
  pinMode(sensorB, INPUT);
}