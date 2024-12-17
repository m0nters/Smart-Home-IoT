#include "bidirectional_entry_detection.h"

// Task for Bidirectional Entry Detection
void taskBidirectionalEntryDetection(void *parameter) {
  while(true) {
    if (digitalRead(sensorA) == HIGH) {
        while (true) {
          if (digitalRead(sensorB) == HIGH) {
            handleVisitorArrival();
            delay(1000);
            visitorCount ? displayMessage(LCD_BIENTRY_DETECTION_SYSTEM, "No movement", "Total: " + String(visitorCount)) : displayMessage(LCD_BIENTRY_DETECTION_SYSTEM, "No visitors", "Total: 0");
            break;
          }
        }
      }

      if (digitalRead(sensorB) == HIGH) {
        while (true) {
          if (digitalRead(sensorA) == HIGH) {
            handleVisitorExit();
            delay(1000);
            visitorCount ? displayMessage(LCD_BIENTRY_DETECTION_SYSTEM, "No movement", "Total: " + String(visitorCount)) : displayMessage(LCD_BIENTRY_DETECTION_SYSTEM, "No visitors", "Total: 0");
            break;
          }
        }
      }
      delay(100); // Yield to other tasks
  }
}