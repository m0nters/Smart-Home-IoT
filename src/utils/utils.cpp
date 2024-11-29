#include "utils.h"

// Function to display messages on the LCD
void displayMessage(LiquidCrystal_I2C& lcd, const String& line1, const String& line2) {
  lcd.clear();
  if (line1 != "") {
    lcd.setCursor(0, 0);
    lcd.print(line1);
  }
  if (line2 != "") {
    lcd.setCursor(0, 1);
    lcd.print(line2);
  }
}