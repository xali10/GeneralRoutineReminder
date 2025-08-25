#ifndef APP_CONTROLLER_H
#define APP_CONTROLLER_H

#define TASKS_CAP 20
#define LED_PIN 2
#define BUTTON_PIN 4
#define BUZZER_PIN 5

#include <Arduino.h>
#include <time.h>
#include <vector>
#include "Led.h"
#include "Button.h"
#include "Speaker.h"
#include "Task.h"
#include "Display.h"


class AppController {

  private:
    // Time
    const char* timezone = "EET-2EEST,M4.5.5/0,M10.5.4/24"; // Eastern Egypt Time
    unsigned long lastNTPUpdate = 0; // Timestamp for the last NTP sync
    const unsigned long ntpSyncInterval = 30 * 60 * 1000; // Sync every 30 minutes (in ms)
    bool alarmFired = false;
    Led testLed;
    Button modeButton;
    Speaker speaker;
    Display display;
    

    std::vector<Task> tasks;
    bool tasksFired[20] = {0};

    
    void syncTime();
    void getCurrentTime(struct tm *_timeinfo);
    void printCurrentTime(struct tm *_timeinfo);
    int checkTimeForTask(struct tm *_timeinfo);
    void handleTaskJob();

  public:
    AppController();

    void init(Display display);
    void update();
    void setTasks(std::vector<Task> tasks);
    void toggleLed();
};

#endif
