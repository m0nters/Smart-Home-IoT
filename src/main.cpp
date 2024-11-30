#include <Arduino.h> 
#include <Keypad.h>
#include <LiquidCrystal_I2C.h>
#include <ESP32Servo.h>
#include "door_lock_system/door_lock_system.h"
#include "bidirectional_entry_detection/bidirectional_entry_detection.h"
#include "light_detection/light_detection.h"
#include "fire_detection/fire_detection.h"
// Pin setup
const int BUZZER = 5;
const int SERVO = 17;
const int BUTTON = 18;
const int sensorA = 23;
const int sensorB = 19;


// LCD setup
LiquidCrystal_I2C LCD_DOOR_LOCK_SYSTEM(0x27, 16, 2);
LiquidCrystal_I2C LCD_BIENTRY_DETECTION_SYSTEM(0x20, 16, 2);

// Servo setup
Servo servo;

// Keypad setup
const byte ROWS = 4;
const byte COLS = 4;
char hexaKeys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
byte rowPins[ROWS] = { 16, 4, 0, 2 };
byte colPins[COLS] = { 15, 8, 7, 6 };
Keypad customKeypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);

// Global variables
int visitorCount = 0;
int entranceScanInterval = 1000;
bool isHomeEntryCompleted = false;
int tryAttempt = 0;
const int maxTryAttempt = 3;
const int passwordLength = 4;
bool isMuted = false;
byte hashedPassword[32];
String passwordPlaceholder = "";

void taskDoorLockSystem(void *parameter);
void taskBidirectionalEntryDetection(void *parameter);
void setup() {
  // Setup for Door Locking and Bidirectional Entry Detection
  LCD_DOOR_LOCK_SYSTEM.init();
  LCD_DOOR_LOCK_SYSTEM.backlight();
  LCD_BIENTRY_DETECTION_SYSTEM.init();
  LCD_BIENTRY_DETECTION_SYSTEM.backlight();
  servo.attach(SERVO);
  pinMode(BUTTON, INPUT);
  pinMode(BUZZER, OUTPUT);
  door_locking_system_init();
  bidirectional_entry_detection_init();

  // Setup for Light Detection
  pinMode(LED_PIN, OUTPUT);
  pinMode(LDR_PIN, INPUT);

  // Set up for Fire Detection
  dhtSensor.setup(DHT_PIN, DHTesp::DHT22);
  pinMode(WATER_PIN, OUTPUT);
  pinMode(BUZZER_FIRE_PIN, OUTPUT);
  FastLED.addLeds<WS2812, NEO_LED_PIN, GRB>(leds, NUM_LEDS);
  FastLED.setBrightness(200); // Set brightness for the LED strip

  // Start tasks
  xTaskCreate(taskDoorLockSystem, "Door Lock System", 2048, NULL, 1, NULL);
  xTaskCreate(taskLightDetection, "Light Detection", 2048, NULL, 1, NULL);
  xTaskCreate(taskFireDetection, "Fire Detection", 2048, NULL, 1, NULL); 
  xTaskCreate(taskBidirectionalEntryDetection, "Bidirectional Entry Detection", 2048, NULL, 1, NULL); 
}

void loop() {
  // The loop is empty as tasks are being handled in the background.
}

// Task for Door Lock System and Bidirectional Entry Detection

void taskDoorLockSystem(void *parameter) {
    while(true) {
      servo.write(0);
      displayMessage(LCD_DOOR_LOCK_SYSTEM, "Welcome home!", "");
      delay(2000);

      bool isPasswordSet = false;
      for (int i = 0; i < 32; i++) {
        if (hashedPassword[i] != 0) {
          isPasswordSet = true;
          break;
        }
      }

      if (!isPasswordSet) {
        setNewPassword();
      } else {
        enterPassword();
      }
        vTaskDelay(100 / portTICK_PERIOD_MS); // Yield to other tasks
    }
}

void taskBidirectionalEntryDetection(void *parameter) {
  while(true) {
    if (digitalRead(sensorA) == HIGH) {
        while (true) {
          if (digitalRead(sensorB) == HIGH) {
            handleVisitorArrival();
            delay(entranceScanInterval);
            visitorCount ? displayMessage(LCD_BIENTRY_DETECTION_SYSTEM, "No movement", "Total: " + String(visitorCount)) : displayMessage(LCD_BIENTRY_DETECTION_SYSTEM, "No visitors", "Total: 0");
            break;
          }
        }
      }

      if (digitalRead(sensorB) == HIGH) {
        while (true) {
          if (digitalRead(sensorA) == HIGH) {
            handleVisitorExit();
            delay(entranceScanInterval);
            visitorCount ? displayMessage(LCD_BIENTRY_DETECTION_SYSTEM, "No movement", "Total: " + String(visitorCount)) : displayMessage(LCD_BIENTRY_DETECTION_SYSTEM, "No visitors", "Total: 0");
            break;
          }
        }
      }
      vTaskDelay(100 / portTICK_PERIOD_MS); // Yield to other tasks
  }
}
