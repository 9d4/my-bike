#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include "secret.h"

#define BLINK_PIN D1
#define RELAY_ON HIGH
#define RELAY_OFF LOW

const char* PARAM_INPUT_1 = "output";
const char* PARAM_INPUT_2 = "state";

AsyncWebServer server(80);

bool blinkning = false;

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html>
<head>
  <title>ESP Web Server</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <style>
    html{font-family:system-ui;display:inline-block;text-align:center;}
    body{margin:0;padding:0;height:100vh;display:grid;align-content:center;justify-content:center;justify-items:center}
    .pushable{background:hsl(340deg 100%% 32%%);border:none;border-radius:12px;padding:0;cursor:pointer;outline-offset:4px}.front{display:block;padding:12px 42px;border-radius:12px;font-size:1.25rem;background:hsl(345deg 100%% 47%%);color:#fff;transform:translateY(-6px)}.pushable:active .front{transform:translateY(-2px)}
    .switch{position:relative;display:inline-block;width:120px;height:68px} 
    .switch input{display:none}
    .slider{position:absolute;top:0;left:0;right:0;bottom:0;background-color:#ccc;border-radius:6px}
    .slider:before{position:absolute;content:"";height:52px;width:52px;left:8px;bottom:8px;background-color:#fff;-webkit-transition:.4s;transition:.4s;border-radius:3px}
    input:checked+.slider{background-color:#b30000}
    input:checked+.slider:before{-webkit-transform:translateX(52px);-ms-transform:translateX(52px);transform:translateX(52px)}
  </style>
</head>
<body>
  <h2>ESP Web Server</h2>
  %COMPONENTS%
<script>function toggleCheckbox(element) {
  var xhr = new XMLHttpRequest();
  if(element.checked){ xhr.open("GET", "/update?output="+element.id+"&state=1", true); }
  else { xhr.open("GET", "/update?output="+element.id+"&state=0", true); }
  xhr.send();
}function blink() {
  var xhr = new XMLHttpRequest();
  xhr.open("GET", "/blink", true);
  xhr.send();
}
</script>
</body>
</html>
)rawliteral";

String outputState(int output){
  if(digitalRead(output)){
    return "checked";
  }
  else {
    return "";
  }
}

// Replaces placeholder with button section in your web page
String htmlProcessor(const String& var){
  //Serial.println(var);
  if(var == "COMPONENTS"){
    String buttons = F("");
    buttons += "<h4>IntLed</h4><label class=\"switch\"><input type=\"checkbox\" onchange=\"toggleCheckbox(this)\" id=\"2\" " + outputState(D4) + "><span class=\"slider\"></span></label>";
    buttons += "<h4>Blink</h4><button class=\"pushable\" onclick=\"blink()\"><span class=\"front\">Blink</span></button>";
    return buttons;
  }

  if (var == "") {
    return String("%");
  }

  return String();
}

// make middleware auth basic 
void auth(AsyncWebServerRequest *request){
  // esp basic auth
  if(!request->authenticate(http_username, http_password)){
    return request->requestAuthentication();
  }
}

bool stillBlinking = false;

void blinkBike() {
  if (stillBlinking)
    return;

  Serial.println("Blinking...");
  stillBlinking = true;

  const char count = 3;
  const char delay_on = 45;
  const char delay_off = 30;

  for (int i = 0; i < count; i++) {
    digitalWrite(BLINK_PIN, RELAY_ON);
    delay(delay_on);
    digitalWrite(BLINK_PIN, RELAY_OFF);
    delay(delay_off);
  }

  // make sure it dies
  digitalWrite(BLINK_PIN, RELAY_OFF);

  blinkning = false;
  stillBlinking = false;
}

void routes() {
  // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    auth(request);
    request->send_P(200, "text/html", index_html, htmlProcessor);
  });

  // blink endpoint
  server.on("/blink", HTTP_GET, [](AsyncWebServerRequest *request){
    auth(request);
    blinkning = true;
    request->send(200, "text/html", "Blinked");
  });

  // Send a GET request to <ESP_IP>/update?output=<inputMessage1>&state=<inputMessage2>
  server.on("/update", HTTP_GET, [] (AsyncWebServerRequest *request) {
    auth(request);
    String inputMessage1;
    String inputMessage2;
    // GET input1 value on <ESP_IP>/update?output=<inputMessage1>&state=<inputMessage2>
    if (request->hasParam(PARAM_INPUT_1) && request->hasParam(PARAM_INPUT_2)) {
      inputMessage1 = request->getParam(PARAM_INPUT_1)->value();
      inputMessage2 = request->getParam(PARAM_INPUT_2)->value();
      digitalWrite(inputMessage1.toInt(), inputMessage2.toInt());
    }
    else {
      inputMessage1 = "No message sent";
      inputMessage2 = "No message sent";
    }
    Serial.print("GPIO: ");
    Serial.print(inputMessage1);
    Serial.print(" - Set to: ");
    Serial.println(inputMessage2);
    request->send(200, "text/plain", "OK");
  });
}

void setup() {
  Serial.begin(115200);
  pinMode(BLINK_PIN, OUTPUT);
  pinMode(D4, OUTPUT);

  digitalWrite(BLINK_PIN, RELAY_OFF);
  digitalWrite(D4, HIGH);

  delay(10);

  // setup wifi ap
  WiFi.setOutputPower(20);
  WiFi.softAP(ssid, password, 1, 1, 4);

  // check that hotspot created
  while (WiFi.softAPIP() == INADDR_NONE) {
    delay(100);
  }

  // print ip
  Serial.print("AP IP address: ");
  Serial.println(WiFi.softAPIP());

  routes();

  // Start server
  server.begin();
}

void loop() {
  if (blinkning) {
    blinkBike();
  }
}
