#ifndef S_LCD_H
#define S_LCD_H

#include <Arduino.h>
#include <LiquidCrystal_I2C.h>

void lcd_init(bool backlight);
void lcd_print(String s, int x, int y);
void lcd_append(String s, int x, int y);
void lcd_turnOff();

#endif
