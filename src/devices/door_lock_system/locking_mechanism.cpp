#include "door_lock_system.h"

// Function to input password
void inputPassword(String& buffer) {
  buffer = ""; // Clear the buffer
  byte inputIndex = 0;
  LCD_DOOR_LOCK_SYSTEM.setCursor(1, 1);

  while (inputIndex < passwordLength) {
    char key = customKeypad.getKey();
    if (!key) continue;
    playTypingSound();

    if (key == '*') {
      // Clear current input
      buffer = "";
      inputIndex = 0;
      LCD_DOOR_LOCK_SYSTEM.setCursor(0, 1);
      LCD_DOOR_LOCK_SYSTEM.print(passwordPlaceholder);
      LCD_DOOR_LOCK_SYSTEM.setCursor(1, 1);
    }
    else if (key == '#') {
      // Handle backspace
      if (inputIndex == 0) continue;
      inputIndex--; // Move index back
      buffer.remove(inputIndex); // Remove last character
      LCD_DOOR_LOCK_SYSTEM.setCursor(1 + inputIndex, 1); // Position cursor at the last character
      LCD_DOOR_LOCK_SYSTEM.print('_'); // Replace character with a placeholder
      LCD_DOOR_LOCK_SYSTEM.setCursor(1 + inputIndex, 1); // Reset cursor position
    }
    else if ((key >= '0' && key <= '9') || (key >= 'A' && key <= 'D')) { // other keys on the keypad
      // Append the character to the buffer
      buffer += key;
      LCD_DOOR_LOCK_SYSTEM.print(key); // Show character briefly
      delay(200); // Wait 200ms
      LCD_DOOR_LOCK_SYSTEM.setCursor(1 + inputIndex, 1); // Move back to overwrite
      LCD_DOOR_LOCK_SYSTEM.print('*'); // Replace with *
      inputIndex++;
    }
  }
}


void setNewPassword() {
  String tempPassword; // Temporary storage for password input
  // using loop to keep setting new password until success
  while (true) {
    // Prompt to set a new password
    displayMessage(LCD_DOOR_LOCK_SYSTEM, "Enter new pass:", passwordPlaceholder);
    inputPassword(tempPassword);
    // Reconfirm the password
    displayMessage(LCD_DOOR_LOCK_SYSTEM, "Confirm pass:", passwordPlaceholder);
    String confirmPassword;
    inputPassword(confirmPassword);
    // Check if passwords match
    if (tempPassword == confirmPassword) {
      // Hash the password
      byte hash[32];
      hashString(hash, tempPassword);
      // Store the hash
      for (int i = 0; i < 32; i++) {
        hashedPassword[i] = hash[i];
      }
      displayMessage(LCD_DOOR_LOCK_SYSTEM, "Pass set success", "Saved!");
      playSuccessSound();
      delay(1300);
      break; // Break out of the password setup loop after success
    }
    else {
      // otherwise setting new password again
      displayMessage(LCD_DOOR_LOCK_SYSTEM, "Passwords don't", "match! Try again");
      playFailureSound();
      delay(1700);
    }
  }
}


void enterPassword() {
  String input; // Buffer for user input
  // Prompt to enter the password
  // using loop to keep entering password until success
  while (true) {
    displayMessage(LCD_DOOR_LOCK_SYSTEM, "Enter password:", passwordPlaceholder);
    inputPassword(input);

    tryAttempt++;
    mqttClient.publish("home-0PPKrXoRcgyppks/tryAttempt", String(tryAttempt).c_str());

    // Check password correctness
    if (validatePassword(input)) break; // Break out of the password entry loop after success

    if (tryAttempt == maxTryAttempt) {
      lockTheSystem();
    }
  }
}


bool validatePassword(const String& input) {
  // Hashed input
  byte hasedInput[32];
  hashString(hasedInput, input);
  // Compare the input hash with the stored hash
  for (int i = 0; i < 32; i++) {
    if (hasedInput[i] != hashedPassword[i]) {
      // Hashes don't match
      if (tryAttempt == maxTryAttempt - 1) {
        displayMessage(LCD_DOOR_LOCK_SYSTEM, "Wrong password", "Last attempt!");
      }
      else if (tryAttempt == maxTryAttempt) {
        displayMessage(LCD_DOOR_LOCK_SYSTEM, "Stop! Max", "attempts reached!");
      }
      else {
        displayMessage(LCD_DOOR_LOCK_SYSTEM, "Wrong password", "Try again!");
      }

      if (tryAttempt != maxTryAttempt) {
        displayMessage(LCD_DOOR_LOCK_SYSTEM, "Attempts left: " + String(maxTryAttempt - tryAttempt), "");
        delay(1000);
      }

      playFailureSound();
      delay(1700);
      return false;
    }
  }
  // Hashes match
  displayMessage(LCD_DOOR_LOCK_SYSTEM, "Door unlocked!", "");

  isDoorLocked = false;
  mqttClient.publish("home-0PPKrXoRcgyppks/isDoorLocked", "false");

  tryAttempt = 0; // Reset the attempt counter
  mqttClient.publish("home-0PPKrXoRcgyppks/tryAttempt", String(tryAttempt).c_str());

  servo.write(90); // Open the door
  playSuccessSound();
  delay(1300);
  while (!isHomeEntryCompleted) {
    awaitDoorClosure(); // Wait for the door to be closed
    servo.write(0); // Close the door

    isDoorLocked = true;
    mqttClient.publish("home-0PPKrXoRcgyppks/isDoorLocked", "true");

    displayMessage(LCD_DOOR_LOCK_SYSTEM, "Door closed!", "");
    isHomeEntryCompleted = true; // User has entered the house, turn off the sound
    playDoorClosedSound();
    delay(1500);
  }
  isHomeEntryCompleted = false; // Reset the entry status
  return true;
}


void lockTheSystem() {
  isDoorPermanentlyLocked = true;
  mqttClient.publish("home-0PPKrXoRcgyppks/isDoorPermanentlyLocked", "true");
  displayMessage(LCD_DOOR_LOCK_SYSTEM, "System locked...", "");
  delay(3000);
  while (isDoorPermanentlyLocked) {
    displayMessage(LCD_DOOR_LOCK_SYSTEM, "Please verify", "online!");
    for (int i = 8; i <= 16; ++i) {
      LCD_DOOR_LOCK_SYSTEM.print(".");
      if (!isDoorPermanentlyLocked) break; // if there's signal from server to unlock the door, then out of lock loop
      vTaskDelay(800 / portTICK_PERIOD_MS); // Yield to other tasks
    }
  }

  tryAttempt = 0; // Reset the attempt counter
  mqttClient.publish("home-0PPKrXoRcgyppks/tryAttempt", String(tryAttempt).c_str());

  displayMessage(LCD_DOOR_LOCK_SYSTEM, "System unlocked!", "");
  delay(3000);
}


void awaitDoorClosure() {
  while (true) {
    displayMessage(LCD_DOOR_LOCK_SYSTEM, "Wait to close", "");
    LCD_DOOR_LOCK_SYSTEM.setCursor(0, 1);
    for (int i = 0; i < 16; ++i) {
      // if at any point receive a signal from button, then close the door
      if (digitalRead(BUTTON) == HIGH) return;
      LCD_DOOR_LOCK_SYSTEM.print(".");
      playWaitingToCloseDoorSound();
      delay(300);
    }
  }
}