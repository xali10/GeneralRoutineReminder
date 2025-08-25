#include "AppController.h"

AppController::AppController() {

}

void AppController::init(Display display) {

  this->display = display;
  testLed.init(LED_PIN);
  modeButton.init(BUTTON_PIN);
  display.init(0x27, 16, 2);
  speaker.init(BUZZER_PIN);
  syncTime();
}

void AppController::syncTime() {
  Serial.print("Synchronizing time with NTP server...");
  configTime(0, 0, "pool.ntp.org", "time.nist.gov"); // UTC offset set to 0
  time_t now = time(nullptr);
  while (now < 24 * 3600) { // Wait until time is valid
    delay(100);
    now = time(nullptr);
  }
  Serial.println(" Time synchronized!");

  // Set timezone
  setenv("TZ", timezone, 1);
  tzset();

  lastNTPUpdate = millis(); // Record the time of the last sync
}

void AppController::update() {

  modeButton.update();
  speaker.loop();
  handleTaskJob();
}



void AppController::getCurrentTime(struct tm *_timeinfo) {

  time_t now = time(nullptr);
  localtime_r(&now, _timeinfo);
}

void AppController::printCurrentTime(struct tm *_timeinfo) {

  Serial.printf("Current time: %02d:%02d:%02d, Date: %04d-%02d-%02d\n",
                _timeinfo->tm_hour,
                _timeinfo->tm_min,
                _timeinfo->tm_sec,
                _timeinfo->tm_year + 1900,
                _timeinfo->tm_mon + 1,
                _timeinfo->tm_mday
               );

}

int AppController::checkTimeForTask(struct tm *_timeinfo) {

  for (int i = 0; i < tasks.size(); i++) {

    if (_timeinfo->tm_hour == tasks.at(i).getHour() && _timeinfo->tm_min == tasks.at(i).getMinute() && !tasksFired[i]) {

      tasksFired[i] = true;

      return i;
    }
  }

  return -1;
}


void AppController::handleTaskJob() {
  unsigned long now = millis();

  struct tm timeinfo;
  getCurrentTime(&timeinfo);

  // Resynchronize with NTP every 30 minutes
  if (now - lastNTPUpdate > ntpSyncInterval) {
    syncTime();
  }

  int taskIndex = checkTimeForTask(&timeinfo);

  if (taskIndex != -1) {
    Serial.println("It's Time To Do Your Job: " + tasks.at(taskIndex).getName());

    display.on();
    display.print("Task:", 0, 0);
    display.print(tasks.at(taskIndex).getName(), 0, 1);

    speaker.play();
    alarmFired = true;
  }

  if (alarmFired && modeButton.isPressed()) {

    Serial.println("Button Pressed!");
    speaker.stop();
    display.off();
    display.clear();
    alarmFired = false;
  }
}

void AppController::setTasks(std::vector<Task> tasks) {

  this->tasks = tasks;
}

void AppController::toggleLed() {
  testLed.toggle();
}


void AppController::connectToWifi(const char* ssid, const char* password) {

  wifiManager.connect(ssid, password);
}
