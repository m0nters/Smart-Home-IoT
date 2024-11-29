#include "bidirectional_entry_detection.h"

void handleVisitorArrival() {
  visitorCount++;
  displayMessage(LCD_BIENTRY_DETECTION_SYSTEM, "Entering...", "Total: " + String(visitorCount));
  tone(BUZZER, 880); // A5
  delay(200);
  noTone(BUZZER);
  delay(100);
  tone(BUZZER, 698); // F5
  delay(200);
  noTone(BUZZER);
}

void handleVisitorExit() {
  visitorCount != 0 ? visitorCount-- : visitorCount = 0;
  displayMessage(LCD_BIENTRY_DETECTION_SYSTEM, "Exiting...", "Total: " + String(visitorCount));
  tone(BUZZER, 440); // A4
  delay(100);
  noTone(BUZZER);
  delay(100);
  tone(BUZZER, 440); // A4
  delay(100);
  noTone(BUZZER);
  delay(100);
  tone(BUZZER, 440); // A4
  delay(100);
  noTone(BUZZER);
}