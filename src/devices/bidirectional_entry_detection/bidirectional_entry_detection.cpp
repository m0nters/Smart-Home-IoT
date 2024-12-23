#include "bidirectional_entry_detection.h"

// Task for Bidirectional Entry Detection
void taskBidirectionalEntryDetection(void* parameter) {
  while (true) {
    if (digitalRead(sensorA) == HIGH) {
      while (true) {
        if (digitalRead(sensorB) == HIGH) {
          handleVisitorArrival();
          vTaskDelay(pdMS_TO_TICKS(1000));
          visitorCount ? displayMessage(LCD_BIENTRY_DETECTION_SYSTEM, "No movement", "Total: " + String(visitorCount)) : displayMessage(LCD_BIENTRY_DETECTION_SYSTEM, "No visitors", "Total: 0");
          break;
        }
        vTaskDelay(pdMS_TO_TICKS(10)); // Add delay in inner loop
      }
    }

    if (digitalRead(sensorB) == HIGH) {
      while (true) {
        if (digitalRead(sensorA) == HIGH) {
          handleVisitorExit();
          vTaskDelay(pdMS_TO_TICKS(1000));
          visitorCount ? displayMessage(LCD_BIENTRY_DETECTION_SYSTEM, "No movement", "Total: " + String(visitorCount)) : displayMessage(LCD_BIENTRY_DETECTION_SYSTEM, "No visitors", "Total: 0");
          break;
        }
        vTaskDelay(pdMS_TO_TICKS(10)); // Add delay in inner loop
      }
    }
    vTaskDelay(pdMS_TO_TICKS(50));
  }
}