#include "bidirectional_entry_detection.h"

extern int entranceScanInterval;
// Task for Bidirectional Entry Detection
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