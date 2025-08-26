#include "Buzzer.h"

Buzzer::Buzzer() {

}


void Buzzer::init(byte pin) {
  this->pin = pin;
  pinMode(pin, OUTPUT);
}


void Buzzer::play() {

  isPlaying = true;
}


void Buzzer::stop() {

  isPlaying = false;
}


void Buzzer::update() {

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

void Buzzer::toggle() {
  isPlaying = !isPlaying;
}
