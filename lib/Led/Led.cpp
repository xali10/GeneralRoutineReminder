#include "Led.h"

Led::Led() {

}

void Led::init(byte pin) {
  this->pin = pin;
  pinMode(pin, OUTPUT);

  off();
}

void Led::on() {

  digitalWrite(pin, HIGH);
}

void Led::off() {

  digitalWrite(pin, LOW);
}

void Led::toggle() {

  digitalWrite(pin, (state? HIGH : LOW));
  state = !state;
}

void Led::update() {
  
}
