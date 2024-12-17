#include "light_detection.h"

// Task for Light Detection

void taskLightDetection(void *parameter) {
  while (true) {
    int lightValue = analogRead(LDR_PIN);
    Serial.println(lightValue);
    if (lightValue > 700) {
      digitalWrite(LED_PIN, HIGH);
    } else {
      digitalWrite(LED_PIN, LOW);
    }

    delay(100); // Run every 100ms
  }
}