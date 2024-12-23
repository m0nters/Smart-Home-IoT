#include "door_lock_system.h"

// Task for Door Lock System 
void taskDoorLockSystem(void* parameter) {
  while (true) {
    if (!isDoorPermanentlyLocked) {
      servo.write(0);
      displayMessage(LCD_DOOR_LOCK_SYSTEM, "Welcome home!", "");
      delay(2000); // Sử dụng vTaskDelay thay vì delay

      for (int i = 0; i < 32; i++) {
        if (hashedPassword[i] != 0) {
          isPasswordSet = true;
          break;
        }
      }

      if (!isPasswordSet) {
        setNewPassword(); // Đảm bảo không blocking
      }
      enterPassword();    // Đảm bảo không blocking
    }
    vTaskDelay(pdMS_TO_TICKS(50));
  }
}
