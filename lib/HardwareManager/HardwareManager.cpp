#include "HardwareManager.h"

HardwareManager::HardwareManager() {

}

void HardwareManager::init(int ledPin, int buttonPin, int buzzerPin) {
  led.init(ledPin);
  button.init(buttonPin);
  buzzer.init(buzzerPin);
}

void HardwareManager::update() {
  button.update();
  led.update(); // TODO
  buzzer.update(); // TODO
}

bool HardwareManager::buttonIsPressed() {
  return button.isPressed();
}

void HardwareManager::ledOn() {
  led.on();
}

void HardwareManager::ledOff() {
  led.off();
}

void HardwareManager::ledToggle() {
  led.toggle();
}

void HardwareManager::buzzerPlay() {
  buzzer.play();
}

void HardwareManager::buzzerStop() {
  buzzer.stop();
}

void HardwareManager::buzzerToggle() {
  buzzer.toggle();
}
