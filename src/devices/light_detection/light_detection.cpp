#include "light_detection.h"

// Task for Light Detection

void taskLightDetection(void* parameter) {
  while (true) {
    int lightValue = analogRead(LDR_PIN);

    int mappedLightValue = 100 - ((lightValue - 32) * 100 / (4063 - 32));

    lightValueForGauge = mappedLightValue;

    if (mappedLightValue < 60 && isAutomaticLight) {
      digitalWrite(LED_PIN, HIGH);
    }
    else {
      digitalWrite(LED_PIN, LOW);
    }

    mqttClient.publish("home-0PPKrXoRcgyppks/lightValue", String(lightValueForGauge).c_str());

    vTaskDelay(pdMS_TO_TICKS(50));
  }
}
