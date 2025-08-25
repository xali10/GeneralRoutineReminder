#include "TimeManager.h"

TimeManager::TimeManager(const char* tz, unsigned long syncInterval)
  : timezone(tz), lastNTPUpdate(0), ntpSyncInterval(syncInterval) {}

void TimeManager::syncTime() {

  unsigned long now = millis();
  // Resynchronize with NTP every 30 minutes
  if (!(now - lastNTPUpdate > ntpSyncInterval))
    return;
  
  Serial.print("Synchronizing time with NTP server...");
  configTime(0, 0, "pool.ntp.org", "time.nist.gov");  // UTC offset set to 0
  time_t now2 = time(nullptr);
  while (now2 < 24 * 3600) {  // Wait until time is valid
    delay(100);
    now2 = time(nullptr);
  }
  Serial.println(" Time synchronized!");

  // Set timezone
  setenv("TZ", timezone, 1);
  tzset();

  lastNTPUpdate = millis();  // Record the time of the last sync
}

void TimeManager::getCurrentTime(struct tm* _timeinfo) {
  
  time_t now = time(nullptr);
  localtime_r(&now, _timeinfo);
}

int TimeManager::checkTimeForTask(struct tm* _timeinfo, std::vector<Task>& tasks, bool* tasksFired) {
  
  for (int i = 0; i < tasks.size(); i++) {
    if (_timeinfo->tm_hour == tasks.at(i).getHour() &&
        _timeinfo->tm_min == tasks.at(i).getMinute() &&
        !tasksFired[i]) {
      tasksFired[i] = true;
      return i;
    }
  }
  return -1;
}
