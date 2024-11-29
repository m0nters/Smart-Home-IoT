#include "door_lock_system.h"

void door_locking_system_init() {
  displayMessage(LCD_DOOR_LOCK_SYSTEM, "Welcome home!", "");
  passwordPlaceholder += "[";
  for (int i = 0; i < passwordLength; i++) {
    passwordPlaceholder += "_";
  }
  passwordPlaceholder += "]";
}