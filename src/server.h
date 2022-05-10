#ifndef SERVER_H
#define SERVER_H

#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include "definition.h"

extern const char* http_username;
extern const char* http_password;

extern AsyncWebServer server;

void startServer();

#endif
