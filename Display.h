#ifndef DISPLAY_H
#define DISPLAY_H

#include <Arduino.h>
#include <LiquidCrystal_I2C.h>


class Display {

  private:
  byte address, cols, rows;
  LiquidCrystal_I2C* lcd;

  public:
  Display(byte address, byte cols, byte rows);
  void init();
  void on();
  void off();
  void print(String s, byte x, byte y);
  void clear();
  
};

#endif
