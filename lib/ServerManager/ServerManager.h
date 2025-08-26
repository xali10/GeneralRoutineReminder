#ifndef SERVER_MANAGER_H
#define SERVER_MANAGER_H

#include <Arduino.h>
#include <WebServer.h>
#include <ArduinoJson.h>


class ServerManager {
  
  private:
    WebServer server;
    void handleReceiveTasks();
    
  public:
    ServerManager();

    void init();
    void update();
};

#endif
