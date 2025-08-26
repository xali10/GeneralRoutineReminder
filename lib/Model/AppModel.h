#ifndef APP_MODEL_H
#define APP_MODEL_H

#include <Arduino.h>
#include <ArduinoJson.h>
#include "SPIFFS.h"
#include <vector>
#include "Task.h"


class AppModel {
  
  private:
    std::vector<Task> tasks;
    
  public:
    AppModel();

    void init();
    void loadTasks(String filename, std::vector<Task>& tasks);
};

#endif
