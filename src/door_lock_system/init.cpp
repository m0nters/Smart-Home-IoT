#include "door_lock_system.h"

void door_locking_system_init() {
  passwordPlaceholder += "[";
  for (int i = 0; i < passwordLength; i++) {
    passwordPlaceholder += "_";
  }
  passwordPlaceholder += "]";
}