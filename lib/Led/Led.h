#ifndef LED_H
#define LED_H

#include <Arduino.h>

class Led {

  private:
    byte pin;
    bool state;

  public:
    Led();
    void init(byte pin);
    void on();
    void off();
    void toggle();
    void update();
  
};

#endif
