#include <Arduino.h>
#include <Wire.h>
#include <WiFiUdp.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ArduinoJson.h>
#include <configuration.h>
#include <index.h>


unsigned int localPort = 2390;

int brightness = 255;
int hue = 0;
int dimmw;
int dimmc;


// A UDP instance to let us send and receive packets over UDP
WiFiUDP udp;



#define SERIALOUT 1

WiFiClient client;
unsigned int last_poll = 0;

ESP8266WebServer server(80);

void handleRoot() {

  String response = String ("<!DOCTYPE html> <script src=\"https://code.jquery.com/jquery-3.1.0.js\"></script> <link href=\"https://fonts.googleapis.com/css?family=Volkhov\" rel=\"stylesheet\"><html><head><title>Schrankbeleuchtung<¹/title></head><body link=black><div> <button id=\"button\" type=\"button\"> <img id=\"bulb\" width=100pt> </button></div><div> Lichtfarbe</div><div> <input id=\"hue\" type=\"range\" min=\"-255\" max=\"255\" value=\"0\" step=\"5\" onchange=\"showHue(this.value)\" \/> <span id=\"huespan\">0<\/span></div><div> Helligkeit</div><div> <input id=\"brightness\" type=\"range\" min=\"0\" max=\"255\" value=\"125\" step=\"5\" onchange=\"showBrightness(this.value)\" /> <span id=\"brightnessspan\">0</span></div> <script type=\"text/javascript\">var url=\"http://simpleicon.com/wp-content/uploads/light-bulb-6.svg\";var on=false;var sliderBrightness=0;var sliderHue=0;var hue=127;var brightness=0;var ip=getQueryVariable(\"ip\");function getQueryVariable(variable){var query=window.location.search.substring(1);var vars=query.split(\"&\");for(var i=0;i<vars.length;i++){var pair=vars[i].split(\"=\");if(pair[0]==variable){return pair[1];}}} $(\"#ip\").html(ip);$(\"#bulb\").attr(\"src\",url) $(\"#button\").on(\"click\",function(){if(on){url=\"http://simpleicon.com/wp-content/uploads/light-bulb-6.svg\";on=false;post();} else{url=\"http://simpleicon.com/wp-content/uploads/light-bulb-7.svg\";on=true;post();} $(\"#bulb\").attr(\"src\",url);}) function showBrightness(newValue) {$(\"#brightnessspan\").html(newValue);sliderBrightness=newValue;brightness=newValue;post();} function showHue(newValue) {$(\"#huespan\").html(newValue);sliderHue=newValue;hue=(215+(sliderHue*(40/125)));$(\"#button\").css(\"background-color\",\"rgb(250,245,\"+hue+\")\") post();} function post(){jQuery.post(\"http:\\"+ip+\"/power\",{value:on,hue:sliderHue,brightness:brightness},function(data,text){console.log(\"result: \",text)});console.log(\"showURL\"+\"http://\"+ip+\"/power\",{value:on,hue:sliderHue,brightness:brightness})}<\/script> <style type=\"style/css\">body{font-size:24pt;font-family:Volkhov;background-color:rgb(255,255,255)}#button{background-color:rgb(255,245,215)}</style></body></html">
  server.send(200, "text/html",response);
  
}

void power() {
  
  Serial.println("getting hue+brightness");  
  hue = server.arg("hue").toInt();
  brightness = server.arg("brightness").toInt();

  Serial.print("Hue:");  
  Serial.println(hue);
  Serial.print("Brightness:");
  Serial.println(brightness);
    

  if(hue < 0 && brightness - hue <= 255){
    dimmw = brightness-hue;
    dimmc = brightness;
  }
  if(hue == 0){
    dimmw = brightness;
    dimmc = brightness;
  }
  if(hue > 0 && brightness + hue <= 255){
    dimmw = brightness;
    dimmc = brightness+hue;
  }
  
  if(server.arg("value")=="true"){
    analogWrite(D3,dimmw);
    analogWrite(D5,dimmc);
  }
  else{
    analogWrite(D3,0);
    analogWrite(D5,0);
  }
  
  Serial.print("Kaltweiss:");
  Serial.println(dimmc);
  Serial.print("Warmweiss: ");
  Serial.println(dimmw);
  
  server.send ( 201, "text/plain", "");
      
}

void getStatus() {
  StaticJsonBuffer<200> jsonBuffer;
  JsonObject& root = jsonBuffer.createObject();
  root["status"] = "OK";
  root["uptime"] = millis();

  String json_string;
  root.printTo(json_string);
  server.send(200, "application/json", json_string);
}

void handleNotFound(){
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET)?"GET":"POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i=0; i<server.args(); i++){
    message += " " + server.argName(0) + server.argName(1) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
}

void setup() {

  pinMode(D0, OUTPUT);
  pinMode(D7, OUTPUT);
  pinMode(D5, OUTPUT);

  
  Wire.begin();

  if (SERIALOUT)
    Serial.begin(9600);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    if (SERIALOUT)
      Serial.print(".");
  }

  if (SERIALOUT) {
    Serial.println("");
    Serial.print("Connected to ");
    Serial.println(ssid);
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
  }

  server.on("/", handleRoot);
  server.on("/power",power);
  server.on("/getStatus", getStatus);

  server.onNotFound(handleNotFound);

  server.begin();
  if (SERIALOUT)
    Serial.println("Template HTTP server started");
Serial.println("Starting UDP");
  udp.begin(localPort);
  Serial.print("Local port: ");
Serial.println(udp.localPort());


}

void loop() {
  server.handleClient();
  unsigned int now = millis();
  if (last_poll + (1000 * 3) < now) {
    if (SERIALOUT) {
        Serial.print("IP address: ");
        Serial.println(WiFi.localIP());
    }
  last_poll = now;

  }
}


