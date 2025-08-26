#include "AppModel.h"
#include "AppController.h"
#include "Display.h"
#include "ServerManager.h"


const char* tasksFile = "/tasks.json";


// ---------------------WiFi------------------
//const char *ssid = ">_";
//const char *password = "Qwertyuio0qwertyuio0";

const char *ssid = "WE_F92510";
const char *password = "12031302";
// -------------------------------------------

AppModel appModel;
AppController controller;
Display display;
ServerManager serverManager;

std::vector<Task> tasks;


void setup() {

  Serial.begin(115200);
  while (!Serial)
    continue;

  appModel.init();
  appModel.loadTasks(tasksFile, tasks);

  display.init(0x27, 16, 2);

  controller.connectToWifi(ssid, password);
  controller.init(display);
  controller.setTasks(tasks);

  serverManager.init();
}

void loop() {

  controller.update();
  serverManager.update();
}
