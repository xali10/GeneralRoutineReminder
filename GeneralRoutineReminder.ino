// 2 (00) , 4 (45min) tasks not ringing

#include "s_wifi.h"

#include "Led.h"
#include "Button.h"
#include "Display.h"
#include "Speaker.h"
#include "AppData.h"


#include <time.h>
#include <WebServer.h>


#define TASKS_CAP 20
#define LED_PIN 2
#define BUTTON_PIN 4
#define BUZZER_PIN 5

const char* tasksFile = "/tasks.json";


// ---------------------WiFi------------------
//const char *ssid = ">_";
//const char *password = "Qwertyuio0qwertyuio0";

const char *ssid = "WE_F92510";
const char *password = "12031302";
// -------------------------------------------


Led testLed(LED_PIN);
Button modeButton(BUTTON_PIN);
Display display(0x27, 16, 2);
Speaker speaker(BUZZER_PIN);
AppData appData;

std::vector<Task> tasks;
bool tasksFired[20] = {0};


WebServer server(80);

// Wifi credintials
// const char* configFile = "/Dexconfig.json";// "/config.json"
// Config config;

// Time
const char* timezone = "EET-2EEST,M4.5.5/0,M10.5.4/24"; // Eastern Egypt Time
unsigned long lastNTPUpdate = 0; // Timestamp for the last NTP sync
const unsigned long ntpSyncInterval = 30 * 60 * 1000; // Sync every 30 minutes (in ms)

// Flags
bool backlightOn = true;
bool backlightChanged = false;
bool alarmFired = false;

void handleReceiveTasks();
void syncTime();

void setup() {

  Serial.begin(115200);
  while (!Serial)
    continue;

  appData.init();

  display.init(); display.off();

  appData.loadTasks(tasksFile, tasks);

  connectToWiFi(ssid, password);

  server.on("/tasks", handleReceiveTasks);
  server.begin();

  syncTime();


}

void loop() {

  modeButton.update();
  speaker.loop();
  server.handleClient();

  handleTaskJob();
}


void getCurrentTime(struct tm *_timeinfo) {

  time_t now = time(nullptr);
  localtime_r(&now, _timeinfo);
}

void printCurrentTime(struct tm *_timeinfo) {

  Serial.printf("Current time: %02d:%02d:%02d, Date: %04d-%02d-%02d\n",
                _timeinfo->tm_hour,
                _timeinfo->tm_min,
                _timeinfo->tm_sec,
                _timeinfo->tm_year + 1900,
                _timeinfo->tm_mon + 1,
                _timeinfo->tm_mday
               );

}

int checkTimeForTask(struct tm *_timeinfo) {

  for (int i = 0; i < tasks.size(); i++) {

    if (_timeinfo->tm_hour == tasks.at(i).getHour() && _timeinfo->tm_min == tasks.at(i).getMinute() && !tasksFired[i]) {

      tasksFired[i] = true;

      return i;
    }
  }

  return -1;
}

void syncTime() {
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


void handleReceiveTasks() {

  if (server.method() == HTTP_POST) {
    String body = server.arg("plain");
    Serial.println(" ---- JSON Received: ---- ");
    Serial.println(body);

    DynamicJsonDocument doc(1024);
    DeserializationError error = deserializeJson(doc, body);
    if (error) {
      Serial.println(" XXXXX JSON parsing failed XXXXX " );
      server.send(400, "text/plain", "Invalid JSON");
      return;
    }

    for (JsonObject task : doc.as<JsonArray>()) {
      Serial.println(" ");
      String name = task["name"].as<String>();
      int hour = task["time"]["hour"];
      int minute = task["time"]["minute"];
      String test = task["time"]["test"];
      Serial.printf("Task: %s at %02d:%02d\n", name.c_str(), hour, minute );
      Serial.println(test);
      Serial.println(" ");
      if (test == "LED-toggle") {
        testLed.toggle();
        Serial.println(">>>>>toggled successfully>>>>>>");
      }
    }

    server.send(200, "text/plain", "Tasks received");
  } else {
    server.send(405, "text/plain", "Method Not Allowed");
  }
}

void handleTaskJob() {
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
