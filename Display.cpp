#include "Display.h"


Display::Display(byte address, byte cols, byte rows) {
  
  this->address = address;
  this->cols = cols;
  this->rows = rows;
}


void Display::init() {

  this->lcd = new LiquidCrystal_I2C(address, cols, rows);
  
  lcd->begin();
  off();
}


void Display::on() {
  
  lcd->backlight();
}


void Display::off() {
  
  lcd->noBacklight();
}


void Display::print(String s, byte x, byte y) {
  
  lcd->setCursor(x, y);
  lcd->print(s);
}

void Display::clear() {

  lcd->clear();
}
