#include "fire_detection.h"

CRGB leds[NUM_LEDS];       // Định nghĩa biến LEDs
DHTesp dhtSensor;          // Định nghĩa biến DHT

unsigned long previousMillis = 0;
unsigned long interval = 500; // Thời gian mỗi chu kỳ (ms)
bool buzzerState = false;     // Trạng thái bật/tắt của buzzer



void taskFireDetection(void *parameter) {
  while (true) {
    // Đọc dữ liệu từ cảm biến DHT
    TempAndHumidity data = dhtSensor.getTempAndHumidity();
    float temperature = data.temperature;
    float humidity = data.humidity;

    // Kiểm tra và gửi MQTT khi nhiệt độ hoặc độ ẩm thay đổi đáng kể
    if (abs(temperature - lastTemperature) >= 0.5) {
      temperatureForGauge = temperature;
      mqttClient.publish("home-0PPKrXoRcgyppks/temperature", String(temperatureForGauge).c_str());
      lastTemperature = temperature;
    }

    if (abs(humidity - lastHumidity) >= 0.5) {
      humidityForGauge = humidity;
      mqttClient.publish("home-0PPKrXoRcgyppks/humidity", String(humidityForGauge).c_str());
      lastHumidity = humidity;
    }

    // Điều khiển WATER_PIN dựa trên điều kiện phun sương
    if (isMistSpray) {
      digitalWrite(WATER_PIN, HIGH);
    } else if (temperature < 40.0 && !isMistSpray) {
      digitalWrite(WATER_PIN, LOW);
    }

    // Điều khiển LEDs dựa trên giá trị nhiệt độ
    if (temperature <= 29.0) {
      noTone(BUZZER_FIRE_PIN);
      digitalWrite(WATER_PIN, LOW);
      fill_solid(leds, NUM_LEDS, CRGB::Green); // Đèn xanh
    } else if (temperature > 29.0 && temperature <= 35.0) {
      // Chuyển từ Green sang Yellow theo nhiệt độ
      int transition = map(temperature, 29, 35, 0, NUM_LEDS);
      for (int i = 0; i < NUM_LEDS; i++) {
        leds[i] = (i < transition) ? CRGB::Yellow : CRGB::Green;
      }
    } else if (temperature > 35.0 && temperature <= 40.0) {
      // Chuyển từ Yellow sang Red theo nhiệt độ
      noTone(BUZZER_FIRE_PIN);
      digitalWrite(WATER_PIN, LOW);
      int transitionToRed = map(temperature, 35, 40, 0, NUM_LEDS);
      for (int i = 0; i < NUM_LEDS; i++) {
        leds[i] = (i < transitionToRed) ? CRGB::Red : CRGB::Yellow;
      }
    } else if (temperature > 40.0) {
      // Kích hoạt cảnh báo khi nhiệt độ cao
      fill_solid(leds, NUM_LEDS, CRGB::Red); // Đèn đỏ toàn bộ
      digitalWrite(WATER_PIN, HIGH);

      if (isFireAlarmSound) {
        unsigned long currentMillis = millis();
        if (currentMillis - previousMillis >= interval) {
          previousMillis = currentMillis;
          buzzerState = !buzzerState;

          if (buzzerState) {
            tone(BUZZER_FIRE_PIN, 2000); // Buzzer kêu
          } else {
            noTone(BUZZER_FIRE_PIN);     // Tắt buzzer
          }
        }
      } else {
          noTone(BUZZER_FIRE_PIN);
      }
    } else {
      // Trường hợp không xác định
      noTone(BUZZER_FIRE_PIN);
      digitalWrite(WATER_PIN, LOW);
      fill_solid(leds, NUM_LEDS, CRGB::Red); // Đèn đỏ
    }

    // Hiển thị LEDs
    FastLED.show();

    // Tạm dừng 50ms trước khi lặp lại
    vTaskDelay(50);
  }
}