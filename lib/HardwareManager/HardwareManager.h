#ifndef HARDWARE_MANAGER_H
#define HARDWARE_MANAGER_H

#include <Arduino.h>
#include "Button.h"
#include "Led.h"
#include "Buzzer.h"


class HardwareManager {

  private:
    int buttonPin, ledPin, buzzerPin;
    Button button;
    Led led;
    Buzzer buzzer;

  public:
    HardwareManager();
    void init(int ledPin, int buttonPin, int buzzerPin);
    void update();
    bool buttonIsPressed();
    void ledOn();
    void ledOff();
    void ledToggle();
    void buzzerPlay();
    void buzzerStop();
    void buzzerToggle();
};

#endif
