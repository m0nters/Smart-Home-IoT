#include "light_detection.h"

void light_detection_init() {
  pinMode(LED_PIN, OUTPUT);
  pinMode(LDR_PIN, INPUT);
}