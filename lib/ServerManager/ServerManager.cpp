#include "ServerManager.h"

ServerManager::ServerManager() {

}

void ServerManager::init() {
  server.on("/tasks", std::bind(&ServerManager::handleReceiveTasks, this));
  server.begin();
}

void ServerManager::update() {

  server.handleClient();
}

void ServerManager::handleReceiveTasks() {
  
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
        // controller.toggleLed(); // TODO
        Serial.println(">>>>>toggled successfully>>>>>>");
      }
      if (test == "puzzer-toggle") {
        // controller.toggleBuzzer(); // TODO
        Serial.println(">>>>>puzzer test successfully>>>>>>");
      }
      
    }

    server.send(200, "text/plain", "Tasks received");
  } else {
    server.send(405, "text/plain", "Method Not Allowed");
  }
}
