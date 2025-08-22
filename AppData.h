#ifndef APP_DATA_H
#define APP_DATA_H

#include <Arduino.h>
#include <ArduinoJson.h>
#include "SPIFFS.h"
#include <vector>
#include "Task.h"


class AppData {
  
  private:
    std::vector<Task> tasks;
    
  public:
    AppData();

    void init();
    void loadTasks(String filename, std::vector<Task>& tasks);
};

#endif
