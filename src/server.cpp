#include <server.h>

const char* PARAM_INPUT_1 = "output";
const char* PARAM_INPUT_2 = "state";

AsyncWebServer server(80);

void auth(AsyncWebServerRequest *request){
  // esp basic auth
  if(!request->authenticate(http_username, http_password)){
    return request->requestAuthentication();
  }
}

String outputState(int output){
  if(digitalRead(output)){
    return "checked";
  }
  else {
    return "";
  }
}

String htmlProcessor(const String& var){
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

void routes() {
  // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    auth(request);
    request->send_P(200, "text/html", index_html, htmlProcessor);
  });

  // blink endpoint
  server.on("/blink", HTTP_GET, [](AsyncWebServerRequest *request) {
    auth(request);
    request->send(200, "text/html", "Blinked");
  });

  // Send a GET request to <ESP_IP>/update?output=<inputMessage1>&state=<inputMessage2>
  server.on("/update", HTTP_GET, [](AsyncWebServerRequest *request) {
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

  server.on("/reboot", HTTP_POST, [](AsyncWebServerRequest *request) {
    ESP.restart();
  });
}

void startServer() {
  routes();
  server.begin();
}
