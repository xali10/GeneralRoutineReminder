

#include <WiFi.h>
#include <WebServer.h>
#include <ArduinoJson.h>

const char *ssid = ">_";
const char *password = "Qwertyuio0qwertyuio0";

int led = 2 ;
bool ledState = false;

WebServer server(80); 

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

void setup() {

  Serial.begin(115200);

  wifiInit();

  pinMode(led, OUTPUT);

  server.on("/tasks", handleReceiveTasks);

  server.begin();
}

void loop() {

  server.handleClient();

}
