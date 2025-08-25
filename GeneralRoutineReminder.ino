#include "AppData.h"
#include "AppController.h"
#include "Display.h"


#include <WebServer.h>


const char* tasksFile = "/tasks.json";


// ---------------------WiFi------------------
//const char *ssid = ">_";
//const char *password = "Qwertyuio0qwertyuio0";

const char *ssid = "WE_F92510";
const char *password = "12031302";
// -------------------------------------------

AppData appData;
AppController controller;
Display display;

std::vector<Task> tasks;

WebServer server(80);


void handleReceiveTasks();


void setup() {

  Serial.begin(115200);
  while (!Serial)
    continue;

  appData.init();
  display.init(0x27, 16, 2);
  display.off();
  

  controller.connectToWifi(ssid, password);

  appData.loadTasks(tasksFile, tasks);
  controller.init(display);
  controller.setTasks(tasks);

  server.on("/tasks", handleReceiveTasks);
  server.begin();
}

void loop() {

  server.handleClient();

  controller.update();
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
        controller.toggleLed();
        Serial.println(">>>>>toggled successfully>>>>>>");
      }
    }

    server.send(200, "text/plain", "Tasks received");
  } else {
    server.send(405, "text/plain", "Method Not Allowed");
  }
}
