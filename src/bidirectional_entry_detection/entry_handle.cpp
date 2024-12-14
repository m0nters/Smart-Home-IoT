#include "bidirectional_entry_detection.h"

void handleVisitorArrival() { // 500ms
  visitorCount++;
  displayMessage(LCD_BIENTRY_DETECTION_SYSTEM, "Entering...", "Total: " + String(visitorCount));
  if (isMutedEntryDetection) {
    delay(500);
    return;
  }
  tone(BUZZER_BidirectionalEntryDetection, 880); // A5
  delay(200);
  noTone(BUZZER_BidirectionalEntryDetection);
  delay(100);
  tone(BUZZER_BidirectionalEntryDetection, 698); // F5
  delay(200);
  noTone(BUZZER_BidirectionalEntryDetection);
}

void handleVisitorExit() { // 500ms
  visitorCount != 0 ? visitorCount-- : visitorCount = 0;
  displayMessage(LCD_BIENTRY_DETECTION_SYSTEM, "Exiting...", "Total: " + String(visitorCount));
  if (isMutedEntryDetection) {
    delay(500);
    return;
  }
  tone(BUZZER_BidirectionalEntryDetection, 440); // A4
  delay(100);
  noTone(BUZZER_BidirectionalEntryDetection);
  delay(100);
  tone(BUZZER_BidirectionalEntryDetection, 440); // A4
  delay(100);
  noTone(BUZZER_BidirectionalEntryDetection);
  delay(100);
  tone(BUZZER_BidirectionalEntryDetection, 440); // A4
  delay(100);
  noTone(BUZZER_BidirectionalEntryDetection);
}