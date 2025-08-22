#include "Speaker.h"

Speaker::Speaker(byte pin) {

  this->pin = pin;
  init();
}


void Speaker::init() {

  pinMode(pin, OUTPUT);
}


void Speaker::play() {

  isPlaying = true;
}


void Speaker::stop() {

  isPlaying = false;
}


void Speaker::loop() {

  if (!isPlaying)
    return;

    digitalWrite(pin, HIGH);
  delay(30);
  digitalWrite(pin, LOW);
  delay(30);
  digitalWrite(pin, HIGH);
  delay(30);
  digitalWrite(pin, LOW);
  delay(30);
  digitalWrite(pin, HIGH);
  delay(30);
  digitalWrite(pin, LOW);
  delay(30);
  digitalWrite(pin, HIGH);
  delay(30);
  digitalWrite(pin, LOW);
  delay(30);
    
//  int size = sizeof(durations) / sizeof(int);

//  for (int note = 0; note < size; note++) {
    
//    int duration = 1000 / durations[note];
//    tone(pin, melody[note], duration);

//    int pauseBetweenNotes = duration * 1.30;
//    delay(pauseBetweenNotes);
    
    //stop the tone playing:
//    noTone(pin);
//  }
  
  
}
