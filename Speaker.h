#ifndef SPEAKER_H
#define SPEAKER_H

#include <Arduino.h>
//#include "pitches.h"


class Speaker {

  private:
    bool isPlaying = false;
    byte pin;
//    int melody[31] = {
//      NOTE_B4, NOTE_B5, NOTE_FS5, NOTE_DS5, NOTE_B5, NOTE_FS5, NOTE_DS5, NOTE_C5, NOTE_C6, NOTE_G6, NOTE_E6, NOTE_C6, NOTE_G6, NOTE_E6,
//      NOTE_B4, NOTE_B5, NOTE_FS5, NOTE_DS5, NOTE_B5, NOTE_FS5, NOTE_DS5, NOTE_DS5, NOTE_E5, NOTE_F5, NOTE_F5, NOTE_FS5, NOTE_G5, NOTE_G5, NOTE_GS5, NOTE_A5, NOTE_B5
//    };
//
//    int durations[31] = {
//      16, 16, 16, 16, 32, 16, 8, 16, 16, 16, 16, 32, 16, 8, 16, 16, 16, 16, 32, 16, 8, 32, 32, 32, 32, 32, 32, 32, 32, 16, 8
//    };

  public:
    Speaker(byte pin);
    void init();
    void play();
    void stop();
    void loop();

};

#endif
