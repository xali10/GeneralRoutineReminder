#ifndef S_WIFI_H
#define S_WIFI_H

#include <Arduino.h>
#include <WiFi.h>



void connectToWiFi(const char* ssid, const char* password) {

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


#endif
