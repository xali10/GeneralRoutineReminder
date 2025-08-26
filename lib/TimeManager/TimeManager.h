#ifndef TIME_MANAGER_H
#define TIME_MANAGER_H

#include <Arduino.h>
#include <time.h>
#include <vector>
#include "Task.h"

class TimeManager {
  private:
    const char* timezone;
    unsigned long lastNTPUpdate;
    const unsigned long ntpSyncInterval;

  public:
    TimeManager(const char* tz, unsigned long syncInterval);
    
    void syncTime();
    void getCurrentTime(struct tm* _timeinfo);
    int checkTimeForTask(struct tm* _timeinfo, std::vector<Task>& tasks, bool* tasksFired);
};

#endif
