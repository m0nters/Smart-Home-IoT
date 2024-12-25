#include "light_detection.h"

// Task for Light Detection
void taskLightDetection(void* parameter) {
  float lastLightValue = -1.0;
  while (true) {

    int lightValue = analogRead(LDR_PIN);

    int mappedLightValue = 100 - ((lightValue - 32) * 100 / (4063 - 32));
    lightValueForGauge = mappedLightValue;

    // Điều kiện bật/tắt đèn dựa trên ánh sáng
    if (mappedLightValue < 60 && isAutomaticLight) {
      digitalWrite(LED_PIN, HIGH);
    } else {
      digitalWrite(LED_PIN, LOW);
    }

    if (abs(mappedLightValue - lastLightValue) >= 0.5) {
      mqttClient.publish("home-0PPKrXoRcgyppks/lightValue", String(lightValueForGauge).c_str());
      lastLightValue = mappedLightValue;
    }

    // Tạm dừng 50ms trước khi lặp lại
    delay(50);
  }
}