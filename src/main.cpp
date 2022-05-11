#include <ESP8266WiFi.h>
#include <secret.h>
#include <server.h>
#include <LittleFS.h>

/* Pins definitions */
#define BELL D1         // GPIO 5
#define AMBER_L D6      // GPIO 12
#define AMBER_R D7      // GPIO 13

#define BELL_HIGH HIGH
#define BELL_LOW LOW

#define AMBER_HIGH LOW
#define AMBER_LOW HIGH

/* Controls */
// determine if the controller is currently has task (e.g. ringing the bell)
bool onProcess = false;
bool beep1 = false;

void beep() {
  if (onProcess)
    return;

  onProcess = true;
  digitalWrite(BELL, BELL_HIGH);
  delay(65);
  digitalWrite(BELL, BELL_LOW);
  onProcess = false;
  beep1 = false;
}

void setup() {
  Serial.begin(115200);

  pinMode(AMBER_L, OUTPUT);
  digitalWrite(AMBER_L, AMBER_LOW);
  pinMode(AMBER_R, OUTPUT);
  digitalWrite(AMBER_R, AMBER_LOW);

  pinMode(BELL, OUTPUT);
  digitalWrite(BELL, BELL_LOW);

  delay(10);

  WiFi.setOutputPower(20);
  WiFi.softAP(ssid, password, 1, 1, 4);

  while (WiFi.softAPIP() == INADDR_NONE) {
    delay(100);
  }

  Serial.print("AP IP address: ");
  Serial.println(WiFi.softAPIP());

  startServer();
}

void loop() {
  if (beep1) {
    beep();
  }
}
