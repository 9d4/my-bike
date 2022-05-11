#include <ESP8266WiFi.h>
#include <secret.h>
#include <server.h>
#include <LittleFS.h>

#define WIFI_SSID_FILE "/wifi_ssid"
#define WIFI_PWD_FILE "/wifi_pwd"
#define WIFI_HIDDEN_FILE "/wifi_hidden"
#define WEB_USER_FILE "/user"
#define WEB_PASS_FILE "/pass"

// default credentials
String wifi_ssid= "traperbike";
String wifi_pass = "123456789";
bool wifi_hidden = 0;
String username = "123456789";
String password = "123456789";

// const char* http_username = "traper";
// const char* http_password = "sutelo001";

/* Pins definitions */
#define BELL D1    // GPIO 5
#define AMBER_L D6 // GPIO 12
#define AMBER_R D7 // GPIO 13

#define BELL_HIGH HIGH
#define BELL_LOW LOW

#define AMBER_HIGH LOW
#define AMBER_LOW HIGH

/* Controls */
// determine if the controller is currently has task (e.g. ringing the bell)
bool onProcess = false;
bool find1 = false;
bool find2 = false;
bool find3 = false;
bool beep1 = false;
bool beep2 = false;
bool ambers1 = false;
bool amberl = false;
bool amberr = false;
bool fancy1 = false;
bool fancy2 = false;
bool fancy3 = false;
bool hazard = false;

void beep()
{
  digitalWrite(BELL, BELL_HIGH);
  delay(65);
  digitalWrite(BELL, BELL_LOW);
}

void beep1Time()
{
  if (onProcess)
    return;

  onProcess = true;
  beep();
  onProcess = false;
  beep1 = false;
}

void beep2Time()
{
  if (onProcess)
    return;

  onProcess = true;
  beep();
  delay(65);
  beep();
  onProcess = false;
  beep2 = false;
}

void blinkAmberL()
{
  digitalWrite(AMBER_L, AMBER_HIGH);
  delay(100);
  digitalWrite(AMBER_L, AMBER_LOW);
}

void blinkAmberR()
{
  digitalWrite(AMBER_R, AMBER_HIGH);
  delay(100);
  digitalWrite(AMBER_R, AMBER_LOW);
}

void blinkAmberL1Time()
{
  if (onProcess)
    return;

  onProcess = true;
  blinkAmberL();
  onProcess = false;
  amberl = false;
}

void blinkAmberR1Time()
{
  if (onProcess)
    return;

  onProcess = true;
  blinkAmberR();
  onProcess = false;
  amberr = false;
}

void blinkAmbers()
{
  digitalWrite(AMBER_L, AMBER_HIGH);
  digitalWrite(AMBER_R, AMBER_HIGH);
  delay(100);
  digitalWrite(AMBER_L, AMBER_LOW);
  digitalWrite(AMBER_R, AMBER_LOW);
}

void blinkAmbers1Time()
{
  if (onProcess)
    return;

  onProcess = true;
  blinkAmbers();
  onProcess = false;
  ambers1 = false;
}

void findStyle1()
{
  if (onProcess)
    return;

  onProcess = true;
  digitalWrite(AMBER_L, AMBER_HIGH);
  digitalWrite(AMBER_R, AMBER_HIGH);
  delay(100);
  digitalWrite(AMBER_L, AMBER_LOW);
  digitalWrite(AMBER_R, AMBER_LOW);
  delay(100);
  digitalWrite(AMBER_L, AMBER_HIGH);
  digitalWrite(AMBER_R, AMBER_HIGH);
  delay(100);
  digitalWrite(AMBER_L, AMBER_LOW);
  digitalWrite(AMBER_R, AMBER_LOW);
  onProcess = false;
  find1 = false;
}

void findStyle2()
{
  if (onProcess)
    return;

  onProcess = true;
  digitalWrite(AMBER_L, AMBER_HIGH);
  digitalWrite(AMBER_R, AMBER_HIGH);
  digitalWrite(BELL, BELL_HIGH);
  delay(65);
  digitalWrite(BELL, BELL_LOW);
  delay(45);
  digitalWrite(AMBER_L, AMBER_LOW);
  digitalWrite(AMBER_R, AMBER_LOW);
  delay(100);
  digitalWrite(AMBER_L, AMBER_HIGH);
  digitalWrite(AMBER_R, AMBER_HIGH);
  delay(100);
  digitalWrite(AMBER_L, AMBER_LOW);
  digitalWrite(AMBER_R, AMBER_LOW);
  onProcess = false;
  find2 = false;
}

void findStyle3()
{
  if (onProcess)
    return;

  onProcess = true;
  digitalWrite(AMBER_L, AMBER_HIGH);
  digitalWrite(AMBER_R, AMBER_HIGH);
  digitalWrite(BELL, BELL_HIGH);
  delay(65);
  digitalWrite(BELL, BELL_LOW);
  delay(45);
  digitalWrite(AMBER_L, AMBER_LOW);
  digitalWrite(AMBER_R, AMBER_LOW);
  delay(20);
  digitalWrite(BELL, BELL_HIGH);
  delay(65);
  digitalWrite(BELL, BELL_LOW);
  delay(15);
  digitalWrite(AMBER_L, AMBER_HIGH);
  digitalWrite(AMBER_R, AMBER_HIGH);
  delay(100);
  digitalWrite(AMBER_L, AMBER_LOW);
  digitalWrite(AMBER_R, AMBER_LOW);
  onProcess = false;
  find3 = false;
}

// Amber left then amber right
void fancyStyle1() {
  if (onProcess)
    return;
  
  onProcess = true;
  digitalWrite(AMBER_L, AMBER_HIGH);
  delay(100);
  digitalWrite(AMBER_L, AMBER_LOW);
  digitalWrite(AMBER_R, AMBER_HIGH);
  delay(100);
  digitalWrite(AMBER_R, AMBER_LOW);
  onProcess = false;
  fancy1 = false;
}

void fancyStyle2() {
  if (onProcess)
    return;
  
  onProcess = true;
  digitalWrite(AMBER_R, AMBER_HIGH);
  delay(100);
  digitalWrite(AMBER_R, AMBER_LOW);
  digitalWrite(AMBER_L, AMBER_HIGH);
  delay(100);
  digitalWrite(AMBER_L, AMBER_LOW);
  onProcess = false;
  fancy2 = false;
}

void fancyStyle3() {
  if (onProcess)
    return;
  
  onProcess = true;
  
  for (char i = 0; i < 3; i++) {
    digitalWrite(AMBER_R, AMBER_HIGH);
    digitalWrite(AMBER_L, AMBER_HIGH);
    delay(80);
    digitalWrite(AMBER_R, AMBER_LOW);
    digitalWrite(AMBER_L, AMBER_LOW);
    delay(60);
  }

  for (char i = 0; i < 3; i++) {
    digitalWrite(AMBER_R, AMBER_HIGH);
    delay(80);
    digitalWrite(AMBER_L, AMBER_HIGH);
    delay(100);
    digitalWrite(AMBER_R, AMBER_LOW);
    delay(80);
    digitalWrite(AMBER_L, AMBER_LOW);
    delay(100);
  }

  for (char i = 0; i < 3; i++) {
    digitalWrite(AMBER_R, AMBER_HIGH);
    digitalWrite(AMBER_L, AMBER_HIGH);
    delay(80);
    digitalWrite(AMBER_R, AMBER_LOW);
    digitalWrite(AMBER_L, AMBER_LOW);
    delay(60);
  }

  onProcess = false;
  fancy3 = false;
}

void blinkHazard() {
  if (onProcess)
    return;

  while (hazard) {
    digitalWrite(AMBER_L, AMBER_HIGH);
    digitalWrite(AMBER_R, AMBER_HIGH);
    delay(250);
    digitalWrite(AMBER_L, AMBER_LOW);
    digitalWrite(AMBER_R, AMBER_LOW);
    delay(250);
  }
  hazard = false;
  onProcess = false;
}

void setup()
{
  Serial.begin(115200);

  pinMode(AMBER_L, OUTPUT);
  digitalWrite(AMBER_L, AMBER_LOW);
  pinMode(AMBER_R, OUTPUT);
  digitalWrite(AMBER_R, AMBER_LOW);

  pinMode(BELL, OUTPUT);
  digitalWrite(BELL, BELL_LOW);

  delay(10);

  // check wheter file named WIFI_SSID_FILE available in littlefs
  LittleFS.begin();

  // WIFI SSID CONFIG 
  File wifiSsidFile = LittleFS.open(WIFI_SSID_FILE, "r");
  if (!wifiSsidFile) {
    // create it
    wifiSsidFile = LittleFS.open(WIFI_SSID_FILE, "w");
    wifiSsidFile.println(wifi_ssid);
    wifiSsidFile.close();
  }

  if (wifiSsidFile) {
    // read the file
    // make buffer
    String buf = wifiSsidFile.readString();
    wifi_ssid = buf;
  }

  // WIFI PASSWORD CONFIG
  File wifiPassFile = LittleFS.open(WIFI_PWD_FILE, "r");
  if (!wifiPassFile) {
    // create it
    wifiPassFile = LittleFS.open(WIFI_PWD_FILE, "w");
    wifiPassFile.println(wifi_pass);
    wifiPassFile.close();
  }

  if (wifiPassFile) {
    // read the file
    // make buffer
    String buf = wifiPassFile.readString();
    wifi_pass = buf;
  }

  // WIFI HIDDEN CONFIG
  File wifiHiddenFile = LittleFS.open(WIFI_HIDDEN_FILE, "r");
  if (!wifiHiddenFile) {
    // create it
    wifiHiddenFile = LittleFS.open(WIFI_HIDDEN_FILE, "w");
    wifiHiddenFile.println(wifi_hidden);
    wifiHiddenFile.close();
  }

  if (wifiHiddenFile) {
    // read the file
    // make buffer
    String buf = wifiHiddenFile.readString();
    wifi_hidden = buf.toInt();
  }

  // USERNAME CONFIG
  File userNameFile = LittleFS.open(WEB_USER_FILE, "r");
  if (!userNameFile) {
    // create it
    userNameFile = LittleFS.open(WEB_USER_FILE, "w");
    userNameFile.println(username);
    userNameFile.close();
  }

  if (userNameFile) {
    // read the file
    // make buffer
    String buf = userNameFile.readString();
    username = buf;
  }

  // PASSWORD CONFIG
  File passwordFile = LittleFS.open(WEB_PASS_FILE, "r");
  if (!passwordFile) {
    // create it
    passwordFile = LittleFS.open(WEB_PASS_FILE, "w");
    passwordFile.println(password);
    passwordFile.close();
  }

  if (passwordFile) {
    // read the file
    // make buffer
    String buf = passwordFile.readString();
    password = buf;
  }

  LittleFS.end();

  WiFi.setOutputPower(20);
  WiFi.softAP(wifi_ssid, wifi_ssid, 1, wifi_hidden, 4);

  while (WiFi.softAPIP() == INADDR_NONE)
  {
    delay(100);
  }

  Serial.print("AP IP address: ");
  Serial.println(WiFi.softAPIP());

  startServer();
}

void loop()
{
  if (beep1) {
    beep1Time();
  }

  if (beep2) {
    beep2Time();
  }

  if (amberl) {
    blinkAmberL1Time();
  }

  if (amberr) {
    blinkAmberR1Time();
  }

  if (ambers1) {
    blinkAmbers1Time();
  }

  if (find1) {
    findStyle1();
  }

  if (find2) {
    findStyle2();
  }

  if (find3) {
    findStyle3();
  }

  if (fancy1) {
    fancyStyle1();
  }

  if (fancy2) {
    fancyStyle2();
  }

  if (fancy3) {
    fancyStyle3();
  }

  if (hazard) {
    blinkHazard();
  }
}
