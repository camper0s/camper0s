//by camper0s

//////////////////////////////////////////////////////////////////////////html
const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html>
  <script src="https://cdnjs.cloudflare.com/ajax/libs/p5.js/1.6.0/p5.js"></script>
   <script type="text/javascript">
/////////////////////////////////////////////////////////////////////Processing P5.js
var arduVal1
var arduVal2
let p5
let r
function setup() {
  createCanvas(700, 700);
  r = random(255);
  g = random(255);
  b = random(255);
textAlign(CENTER);
textSize(width/30);
r =0;
p5=0;
arduVal1=0;
arduVal2=0;
 }
function draw() {
var source = new EventSource('/events');
source.addEventListener('newVal', function(e) {
const obj = JSON.parse(e.data);
arduVal1 = obj.out1.toFixed(2); // toFixed(2) =  2 decimal
arduVal2 = obj.out2;
});

background(200,50,20);
noStroke();
fill(20,20,255);
text ("from ardu1 (5s)", width/20, width/14);
text (arduVal1, width/20, width/9);
text ("from ardu2 (5s)", width/2.9, width/14);
text (arduVal2, width/2.9, width/9);

strokeWeight(2);
stroke(r, g, b);
fill(r, g, b, 127);
ellipse(width/2, width/2, width/3.6, width/3.6);

push();
translate(width / 2, width / 2);
fill(0,255,0);
r = r+1;
rotate(radians(r));
rect(-26, -26, 52, 52);
pop();

stroke(255,255,0);
text ("clickMe", width/2, width/2);
}

function mousePressed() {
  // Check if mouse is inside the circle
  let d = dist(mouseX, mouseY, width/2, width/2);
  if (d < 100) {
    // Pick new random color values
    r = random(255);
    g = random(255);
    b = random(255);
  }
}
////////////////////////////////////////////////////////////////////////html
</script>
</html>)rawliteral";
////////////////////////////////////////////////////////////////////arduino esp8266
#include <espnow.h>
#include <ESP8266WiFi.h>
#include "ESPAsyncWebServer.h"
#include "ESPAsyncTCP.h"
#include <Arduino_JSON.h>
const char *ssid = "TIM-37665000";
const char *password = "sipavavekimie";
/////////////////
float ardu1;
int ardu2;
AsyncWebServer server(80);
AsyncEventSource events("/events");

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_AP_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Setting as a Wi-Fi Station..");
  }
  Serial.print("Station IP Address: ");
  Serial.println(WiFi.localIP());
  Serial.print("Wi-Fi Channel: ");
  Serial.println(WiFi.channel());
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send_P(200, "text/html", index_html);
  });
  server.addHandler(&events);
  server.begin();
  delay(4000);
}

void loop() {
  ardu1 = ardu1 + 0.01;
  ardu2 = random(1, 1234);
  JSONVar myVal;
  myVal["out1"] = ardu1;
  myVal["out2"] = ardu2;
  String jsonStr = JSON.stringify(myVal);
  events.send(jsonStr.c_str(), "newVal", millis(), 5000);
  events.send("ping", NULL, millis(), 5000);
}
