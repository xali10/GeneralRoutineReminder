#include "s_lcd.h"

LiquidCrystal_I2C lcd (0x27, 16, 2);

void lcd_init(bool backlight) {

  lcd.begin();
  backlight? lcd.backlight() : lcd.noBacklight();
}

void lcd_print(String s, int x, int y) {

  lcd.backlight();
  lcd.clear();
  lcd.setCursor(x, y);
  lcd.print(s);
}

void lcd_append(String s, int x, int y) {

  lcd.backlight();
  lcd.setCursor(x, y);
  lcd.print(s);
}

void lcd_turnOff() {

  lcd.noBacklight();
}
