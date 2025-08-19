#include <WiFi.h>
#include <ArduinoJson.h>
#include "SPIFFS.h"
#include <time.h>
#include <LiquidCrystal_I2C.h>

#define TASKS_CAP 20


struct Config {
  char ssid[64];
  char password[64];
};

struct Time {
  int hour;
  int minute;
};

struct Task {
  char name[64];
  Time time;
};


// Wifi credintials
const char* configFile = "/config.json";
Config config;

// Tasks
const char* tasksFile = "/tasks.json";
Task tasks[TASKS_CAP];
bool tasksFired[TASKS_CAP] = {0};

// Time
const char* timezone = "EET-2EEST,M4.5.5/0,M10.5.4/24";
unsigned long lastNTPUpdate = 0; // Timestamp for the last NTP sync
const unsigned long ntpSyncInterval = 30 * 60 * 1000; // Sync every 30 minutes (in ms)

// LCD
LiquidCrystal_I2C lcd(0x27,16,2);

const int buttonPin = 4;  // the number of the pushbutton pin
const int ringPin =  5;    // the number of the puzzer pin
bool backlightOn = true;
bool backlightChanged = false;
bool alarmFired = false;

void setup() {
  
  Serial.begin(115200);
  while (!Serial)
    continue;

  SPIFFS.begin();
  loadConfiguration(configFile, config);
  loadTasks(tasksFile, tasks);

  connectToWiFi(config.ssid, config.password);

  lcd.init();
  lcd.backlight();

  syncTime();

  pinMode(buttonPin, INPUT);
  pinMode(ringPin, OUTPUT);
}

void loop() {
  
  struct tm timeinfo;
  getCurrentTime(&timeinfo);
  
  printCurrentTime(&timeinfo);

  // Resynchronize with NTP every 30 minutes
  if (millis() - lastNTPUpdate > ntpSyncInterval) {
    syncTime();
  }

  int taskIndex = checkTimeForTask(&timeinfo);

  if (taskIndex != -1) {

    Serial.println("It's Time To Do Your Job: " + String(tasks[taskIndex].name));

    lcd.backlight();
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Task:");
    lcd.setCursor(0,1);
    lcd.print(String(tasks[taskIndex].name));

    digitalWrite(ringPin, HIGH);

    alarmFired = true;
  }

  if (alarmFired) {

    if (digitalRead(buttonPin) == HIGH) {
      
      digitalWrite(ringPin, LOW);
      lcd.noBacklight();
    
      alarmFired = false;
    }
  }

  delay(1000);
}


// Loads the configuration from a file
void loadConfiguration(const char* filename, Config& config) {
  
  File file = SPIFFS.open(filename);
  if(file)
    Serial.println("config.json is read");

  JsonDocument doc;
  DeserializationError error = deserializeJson(doc, file);
  if (error)
    Serial.println(F("Failed to read file"));
  
  strlcpy(config.ssid,          
          doc["ssid"] | "null", 
          sizeof(config.ssid)); 
          
  strlcpy(config.password,      
          doc["password"] | "empty", 
          sizeof(config.password));  

  file.close();

  Serial.println("ssid: " + String(config.ssid) + "\npassword: " + String(config.password));
}


// Load tasks from file
void loadTasks(const char* filename, Task tasks[]) {

  File file = SPIFFS.open(filename);

  StaticJsonDocument<512> doc;
  DeserializationError error = deserializeJson(doc, file);
  if (error) {
    Serial.print("deserializeJson() failed: ");
    Serial.println(error.c_str());
    return;
  }

  int i = 0;
  for (JsonObject item : doc.as<JsonArray>()) {

    Task task;
    strcpy(task.name, item["name"]);
    Time time;
    time.hour = item["time"]["hour"];
    time.minute = item["time"]["minute"];
    task.time = time;

    tasks[i] = task;
  
    Serial.println("name: " + String(task.name) + ", hour:" + String(time.hour));
    
    i++;
  }

  file.close();
}


void getCurrentTime(struct tm *_timeinfo) {

  time_t now = time(nullptr);
  localtime_r(&now, _timeinfo);
}

void printCurrentTime(struct tm *_timeinfo) {

  Serial.printf("Current time: %02d:%02d:%02d, Date: %04d-%02d-%02d\n",
              _timeinfo->tm_hour, _timeinfo->tm_min, _timeinfo->tm_sec,
              _timeinfo->tm_year + 1900, _timeinfo->tm_mon + 1, _timeinfo->tm_mday);

}

int checkTimeForTask(struct tm *_timeinfo) {

  for (int i = 0; i < TASKS_CAP; i++) {

    Task task = tasks[i];
    Time time = tasks[i].time;
    
    if (_timeinfo->tm_hour == time.hour && _timeinfo->tm_min == time.minute && !tasksFired[i]) {
      
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

void connectToWiFi(const char*ssid, const char* password) {

  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected.");
}
// ---------------------------------------------------------
void toggleLed() {
  Serial.print(" ledState : ");
  Serial.println(ledState);

  ledState = !ledState; 
  digitalWrite(led, ledState ? HIGH : LOW);
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
      if(hour == 1){
        toggleLed();
      }

    }

    server.send(200, "text/plain", "Tasks received");
  } else {
    server.send(405, "text/plain", "Method Not Allowed");
  }
}

void wifiInit(){

  WiFi.begin(ssid, password);

  Serial.print("Connecting to WiFi...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println(" Connected!");
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());

}