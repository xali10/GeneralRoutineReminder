#include "AppModel.h"

AppModel::AppModel() {

  
}

void AppModel::init() {

  Serial.println("1");
  SPIFFS.begin();
}

void AppModel::loadTasks(String filename, std::vector<Task>& tasks) {

  Serial.println("2");
  File file = SPIFFS.open(filename.c_str());

  StaticJsonDocument<512> doc;
  DeserializationError error = deserializeJson(doc, file);
  if (error) {
    Serial.print("deserializeJson() failed: ");
    Serial.println(error.c_str());
    return;
  }

  for (JsonObject item : doc.as<JsonArray>()) {

    Task task(item["name"], item["time"]["hour"], item["time"]["minute"]);

    this->tasks.push_back(task);
  }

  file.close();

  tasks = this->tasks;
  Serial.println("3");
}
