#include "Task.h"


Task::Task(String name, int hour, int minute) {

  this->name = name;
  this->hour = hour;
  this->minute = minute;
}

void Task::setName(String name) {

  this->name = name;
}

void Task::setHour(int hour) {

  this->hour = hour;
}

void Task::setMinute(int minute) {

  this->minute = minute;
}

String Task::getName() {

  return name;
}

int Task::getHour() {

  return hour;
}

int Task::getMinute() {

  return minute;
}
