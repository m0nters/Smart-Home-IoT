#include "light_detection.h"

// Task for Light Detection

void taskLightDetection(void *parameter) {
  while (true) {
    int lightValue = analogRead(LDR_PIN);
    if (lightValue > 700) {
      digitalWrite(LED_PIN, HIGH);
    } else {
      digitalWrite(LED_PIN, LOW);
    }

    vTaskDelay(100 / portTICK_PERIOD_MS); // Run every 100ms
  }
}
