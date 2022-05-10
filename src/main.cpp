#include <ESP8266WiFi.h>
#include "secret.h"
#include "server.h"

bool blinkning = false;

bool stillBlinking = false;

// void blinkBike() {
//   if (stillBlinking)
//     return;

//   Serial.println("Blinking...");
//   stillBlinking = true;

//   const char count = 3;
//   const char delay_on = 45;
//   const char delay_off = 30;

//   for (int i = 0; i < count; i++) {
//     digitalWrite(BLINK_PIN, RELAY_ON);
//     delay(delay_on);
//     digitalWrite(BLINK_PIN, RELAY_OFF);
//     delay(delay_off);
//   }

//   // make sure it dies
//   digitalWrite(BLINK_PIN, RELAY_OFF);

//   blinkning = false;
//   stillBlinking = false;
// }

void setup() {
  Serial.begin(115200);

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
}
