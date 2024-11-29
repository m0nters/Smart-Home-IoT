#include "door_lock_system.h"

// Function to input password
void inputPassword(String& buffer) {
  buffer = ""; // Clear the buffer
  byte inputIndex = 0;
  lcd.setCursor(1, 1);

  while (inputIndex < passwordLength) {
    char key = customKeypad.getKey();
    if (!key) continue;
    playTypingSound();

    if (key == '*') {
      // Clear current input
      buffer = "";
      inputIndex = 0;
      lcd.setCursor(0, 1);
      lcd.print(passwordPlaceholder);
      lcd.setCursor(1, 1);
    }
    else if (key == '#') {
      // Handle backspace
      if (inputIndex == 0) continue;
      inputIndex--; // Move index back
      buffer.remove(inputIndex); // Remove last character
      lcd.setCursor(1 + inputIndex, 1); // Position cursor at the last character
      lcd.print('_'); // Replace character with a placeholder
      lcd.setCursor(1 + inputIndex, 1); // Reset cursor position
    }
    else if ((key >= '0' && key <= '9') || (key >= 'A' && key <= 'D')) { // other keys on the keypad
      // Append the character to the buffer
      buffer += key;
      lcd.print(key); // Show character briefly
      delay(200); // Wait 200ms
      lcd.setCursor(1 + inputIndex, 1); // Move back to overwrite
      lcd.print('*'); // Replace with *
      inputIndex++;
    }
  }
}


void setNewPassword() {
  String tempPassword; // Temporary storage for password input
  // using loop to keep setting new password until success
  while (true) {
    // Prompt to set a new password
    displayMessage("Enter new pass:", passwordPlaceholder);
    inputPassword(tempPassword);
    // Reconfirm the password
    displayMessage("Confirm pass:", passwordPlaceholder);
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
      displayMessage("Pass set success", "Saved!");
      playSuccessSound();
      delay(1300);
      break; // Break out of the password setup loop after success
    }
    else {
      // otherwise setting new password again
      displayMessage("Passwords don't", "match! Try again");
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
    displayMessage("Enter password:", passwordPlaceholder);
    inputPassword(input);
    tryAttempt++;
    // Check password correctness
    if (validatePassword(input)) break; // Break out of the password entry loop after success

    if (tryAttempt == maxTryAttempt) {
      lockTheSystem();
    }
    else {
      displayMessage("Attempts left: " + String(maxTryAttempt - tryAttempt), "");
      delay(1000);
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
        displayMessage("Wrong password", "Last attempt!");
      }
      else if (tryAttempt == maxTryAttempt) {
        displayMessage("Stop! Max", "attempts reached!");
      }
      else {
        displayMessage("Wrong password", "Try again!");
      }
      playFailureSound();
      delay(1700);
      return false;
    }
  }
  // Hashes match
  displayMessage("Door unlocked!", "");
  tryAttempt = 0;
  servo.write(90);
  playSuccessSound();
  delay(1300);
  while (!isHomeEntryCompleted) {
    awaitDoorClosure();
    displayMessage("Door closed!", "");
    servo.write(0);
    isHomeEntryCompleted = true;
    playDoorClosedSound();
    delay(1500);
  }
  isHomeEntryCompleted = false;
  return true;
}


void lockTheSystem() {
  displayMessage("System locked...", "");
  delay(3000);
  while (true) {
    displayMessage("Please verify", "online!");
    for (int i = 8; i <= 16; ++i) {
      lcd.print(".");
      delay(800);
    }
  }
}


void awaitDoorClosure() {
  while (true) {
    displayMessage("Wait to close", "");
    lcd.setCursor(0, 1);
    for (int i = 0; i < 16; ++i) {
      // if at any point receive a signal from button, then close the door
      if (digitalRead(BUTTON) == HIGH) return;
      lcd.print(".");
      playWaitingToCloseDoorSound();
      delay(300);
    }
  }
}