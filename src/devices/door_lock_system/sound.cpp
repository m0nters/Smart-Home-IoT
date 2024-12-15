#include "door_lock_system.h"

// Function to play failure sound (set password failed, wrong password)
void playFailureSound() { // 300ms
  if (isMutedDoorSystem) {
    delay(300);
    return;
  }
  for (int i = 0; i < 3; i++) {
    tone(BUZZER_DoorLockSystem, 1000); // High-pitched beep
    delay(150);
    noTone(BUZZER_DoorLockSystem);
    delay(100);
  }
}


void playTypingSound() { // 50ms
  if (isMutedDoorSystem) {
    delay(50);
    return;
  }
  tone(BUZZER_DoorLockSystem, 880); // Frequency: 880 Hz (pleasant high tone)
  delay(50);         // Short duration for the "click" effect
  noTone(BUZZER_DoorLockSystem);    // Stop the tone immediately after
}


// Function to play success sound (password set successfully, correct password entered)
void playSuccessSound() { // 700ms
  if (isMutedDoorSystem) {
    delay(700);
    return;
  }
  tone(BUZZER_DoorLockSystem, 523); // C5
  delay(200);
  tone(BUZZER_DoorLockSystem, 659); // E5
  delay(200);
  tone(BUZZER_DoorLockSystem, 784); // G5
  delay(300);
  noTone(BUZZER_DoorLockSystem);
}


void playDoorClosedSound() { // 500ms
  if (isMutedDoorSystem) {
    delay(500);
    return;
  }
  tone(BUZZER_DoorLockSystem, 880); // A5
  delay(200);
  noTone(BUZZER_DoorLockSystem);
  delay(100);
  tone(BUZZER_DoorLockSystem, 698); // F5
  delay(200);
  noTone(BUZZER_DoorLockSystem);
}


void playWaitingToCloseDoorSound() { // 500ms
  // for security reasons, this sound should not be affected by isMuted option
  tone(BUZZER_DoorLockSystem, 880); // High tone
  delay(150);
  noTone(BUZZER_DoorLockSystem);    // Pause
  delay(100);
  tone(BUZZER_DoorLockSystem, 440); // Low tone
  delay(150);
  noTone(BUZZER_DoorLockSystem);    // Pause
  delay(500);        // Longer pause between sequences
}