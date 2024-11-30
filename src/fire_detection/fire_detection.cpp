#include "fire_detection.h"

CRGB leds[NUM_LEDS];       // Định nghĩa biến LEDs
DHTesp dhtSensor;          // Định nghĩa biến DHT

unsigned long previousMillis = 0;
unsigned long interval = 500; // Thời gian mỗi chu kỳ (ms)
bool buzzerState = false;     // Trạng thái bật/tắt của buzzer

void taskFireDetection(void * parameter) {
  while (true) {
    TempAndHumidity data = dhtSensor.getTempAndHumidity();
    float temperature = data.temperature;

    Serial.println("Temp: " + String(temperature, 2) + "℃");
    Serial.println("---");

    if (temperature <= 29.0) {
      noTone(BUZZER_FIRE_PIN);
      digitalWrite(WATER_PIN, LOW);
      fill_solid(leds, NUM_LEDS, CRGB::Green);
    } else if (temperature > 29.0 && temperature <= 35.0) {
      int transition = map(temperature, 29, 35, 0, NUM_LEDS); 
      for (int i = 0; i < NUM_LEDS; i++) {
        leds[i] = (i < transition) ? CRGB::Yellow : CRGB::Green;
      }
    } else if (temperature > 35.0 && temperature <= 40.0) {
      noTone(BUZZER_FIRE_PIN);
      digitalWrite(WATER_PIN, LOW);
      int transitionToRed = map(temperature, 35, 40, 0, NUM_LEDS); 
      for (int i = 0; i < NUM_LEDS; i++) {
        leds[i] = (i < transitionToRed) ? CRGB::Red : CRGB::Yellow;
      }
    } else if (temperature > 50.0) {
      fill_solid(leds, NUM_LEDS, CRGB::Red);
      digitalWrite(WATER_PIN, HIGH);

      // Điều khiển buzzer ngắt quãng
      unsigned long currentMillis = millis();
      if (currentMillis - previousMillis >= interval) {
        previousMillis = currentMillis;  // Cập nhật thời gian trước đó
        buzzerState = !buzzerState;      // Chuyển trạng thái buzzer

        if (buzzerState) {
          tone(BUZZER_FIRE_PIN, 2000);  // Buzzer kêu
        } else {
          noTone(BUZZER_FIRE_PIN);      // Tắt buzzer
        }
      }
    } else {
      noTone(BUZZER_FIRE_PIN);
      digitalWrite(WATER_PIN, LOW);
      fill_solid(leds, NUM_LEDS, CRGB::Red);
    }

    FastLED.show();
    vTaskDelay(50 / portTICK_PERIOD_MS); // Run every 1ms
  }
}
