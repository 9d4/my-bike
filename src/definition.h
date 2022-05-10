#ifndef DEFINITION_H
#define DEFINITION_H

#ifndef WiFi_h
#include <ESP8266WiFi.h>
#endif

/* 
    Pins definitions
*/
#define BLINK_PIN D1
#define RELAY_ON HIGH
#define RELAY_OFF LOW

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

#endif
