#include "fire_detection.h"

void fire_detection_init() {
  dhtSensor.setup(DHT_PIN, DHTesp::DHT22);
  pinMode(WATER_PIN, OUTPUT);
  pinMode(BUZZER_FIRE_PIN, OUTPUT);
  FastLED.addLeds<WS2812, NEO_LED_PIN, GRB>(leds, NUM_LEDS);
  FastLED.setBrightness(200); // Set brightness for the LED strip
}