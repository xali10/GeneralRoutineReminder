#ifndef TASK_H
#define TASK_H

#include <Arduino.h>


class Task {
  
  private:
    String name;
    int hour;
    int minute;
    
  public:
    Task(String name, int hour, int minute);

    void setName(String name);
    void setHour(int hour);
    void setMinute(int minute);
    String getName();
    int getHour();
    int getMinute();
};

#endif
