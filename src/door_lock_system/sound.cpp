#include "door_lock_system.h"

// Function to play failure sound (set password failed, wrong password)
void playFailureSound() { // 300ms
  if (isMuted) {
    delay(300);
    return;
  }
  for (int i = 0; i < 3; i++) {
    tone(BUZZER, 1000); // High-pitched beep
    delay(150);
    noTone(BUZZER);
    delay(100);
  }
}


void playTypingSound() { // 50ms
  if (isMuted) {
    delay(50);
    return;
  }
  tone(BUZZER, 880); // Frequency: 880 Hz (pleasant high tone)
  delay(50);         // Short duration for the "click" effect
  noTone(BUZZER);    // Stop the tone immediately after
}


// Function to play success sound (password set successfully, correct password entered)
void playSuccessSound() { // 700ms
  if (isMuted) {
    delay(700);
    return;
  }
  tone(BUZZER, 523); // C5
  delay(200);
  tone(BUZZER, 659); // E5
  delay(200);
  tone(BUZZER, 784); // G5
  delay(300);
  noTone(BUZZER);
}


void playDoorClosedSound() { // 500ms
  if (isMuted) {
    delay(500);
    return;
  }
  tone(BUZZER, 880); // A5
  delay(200);
  noTone(BUZZER);
  delay(100);
  tone(BUZZER, 698); // F5
  delay(200);
  noTone(BUZZER);
}


void playWaitingToCloseDoorSound() { // 500ms
  // for security reasons, this sound should not be affected by isMuted option
  tone(BUZZER, 880); // High tone
  delay(150);
  noTone(BUZZER);    // Pause
  delay(100);
  tone(BUZZER, 440); // Low tone
  delay(150);
  noTone(BUZZER);    // Pause
  delay(500);        // Longer pause between sequences
}