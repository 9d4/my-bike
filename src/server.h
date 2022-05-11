#ifndef SERVER_H
#define SERVER_H

#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>

extern String wifi_ssid;
extern String wifi_pass;
extern bool wifi_hidden; 
extern String username;
extern String password;

extern AsyncWebServer server;

void startServer();

#endif
