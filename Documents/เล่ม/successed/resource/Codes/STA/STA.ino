#include <Arduino.h>
#include "heltec.h"
#ifdef ESP32
#include <WiFi.h>
#include <AsyncTCP.h>
#else
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#endif
#include <ESPAsyncWebServer.h>

AsyncWebServer server(80);

#define BAND    433E6  //you can set band here directly,e.g. 868E6,915E6
String x;
const char* ipAddr = "X0";
String node1 = "X1";
String node;
int sw = 13;
int sw1 = 2;
int sw2 = 15;
float temp ;
float humi;
int setTemp;
int setHumi;
bool ctrlMode;
bool pumpState;
bool fanState;
String ctrlModeStr, pumpStateStr, fanStateStr;
String ctrlModeStrWeb, pumpStateStrWeb, fanStateStrWeb;

bool webChangeMode, webPump, webFan; // control via web

unsigned long previousMillis = 0;        // will store last time LED was updated

// constants won't change:
const long interval = 30000;


// REPLACE WITH YOUR NETWORK CREDENTIALS
const char* ssid = "ASUS";
const char* password = "123456789";

String inputMessage;
String inputParam;
const char* PARAM_INPUT_1 = "input1"; //temp
const char* PARAM_INPUT_2 = "input2"; //humi

void notFound(AsyncWebServerRequest *request) {
  request->send(404, "text/plain", "Not found");
}

void setup() {
  Serial.begin(115200);
  pinMode (sw, INPUT_PULLUP);
  pinMode (sw1, INPUT_PULLUP);
  pinMode (sw2, INPUT_PULLUP);

  //WIFI Kit series V1 not support Vext control
  Heltec.begin(false /*DisplayEnable Enable*/, true /*Heltec.LoRa Disable*/, true /*Serial Enable*/, true /*PABOOST Enable*/, BAND /*long BAND*/);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.print("connecting to...");
  Serial.println(ssid);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());


  // Send web page with input fields to client
   server.on("/", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send(200, "text/html", "<!DOCTYPE HTML><html><head>"
                  "<title>ESP Input Form</title>"
                  "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">"
                  "<style>body{text-align:center;}"
                  ".button {background-color: #4CAF50;border: none;color: white;padding: 12px 32px;"
                  "text-align: center;text-decoration: none;display: inline-block;font-size: 16px; margin: 4px 2px; cursor: pointer;}"
                  ".button2 {background-color: #008CBA;}"
                  ".button3 {background-color: #f44336;} /* Red */"
                  ".button4 {background-color: #e7e7e7; color: black;} /* Gray */"
                  ".button5 {background-color: #555555;} /* Black */"
                  "</style>"
                  "</head><body>"
                  "<h2>WiFi LoRa 32 Web Server</h2>"
                  "<form action=\"/get\">"
                  "SETTemp: <input type=\"number\" name=\"input1\">"
                  "<input type=\"submit\" value=\"Submit\">"
                  "</form><br>"
                  "<form action=\"/get\">"
                  "SETHumi: <input type=\"number\" name=\"input2\">"
                  "<input type=\"submit\" value=\"Submit\">"
                  "</form><p>TemperatureLimit: " + String(setTemp) + " *C</p>"
                  "<p>HumidityLimit: " + String(setHumi) + " %</p>"
                  "<p>Temperature: " + String(temp) + " *C</p>"
                  "<p>Humidity: " + String(humi) + " %</p>"
                  "<p>PumpState: " + String(pumpStateStr) + "</p>"
                  "<p>FanState: " + String(fanStateStr) + " </p>"
                  "<p>Mode: " + String(ctrlModeStr) + " </p>"
                  "<a href=\"/P\" class = \"button button3\">pump on/off</a>"
                  "<a href=\"/F\" class = \"button button3\">fan on/off</a>"
                  "<a href=\"/M\" class = \"button button2\">change mode</a><br>"
                  "<a href=\"/\" class = \"button\">refresh</a>"
                  "</body></html>"
                 );
  });
  
  server.on("/F", HTTP_GET, [](AsyncWebServerRequest * request) {
    webFan = true;
    request->send(200, "text/html", "<!DOCTYPE HTML><html><head>"
                  "<title>ESP Input Form</title>"
                  "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">"
                  "<style>body{text-align:center;}"
                  "</style>"
                  "</head><body>"
                  "<p>FanState: " + String(fanStateStrWeb) + " </p>"
                  "<br><a href=\"/\">Return to Home Page</a>"
                  "</body></html>"
                 );
  });
  
  server.on("/P", HTTP_GET, [](AsyncWebServerRequest * request) {
    webPump = true;
   request->send(200, "text/html", "<!DOCTYPE HTML><html><head>"
                  "<title>ESP Input Form</title>"
                  "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">"
                  "<style>body{text-align:center;}"
                  "</style>"
                  "</head><body>"
                  "<p>pumpState: " + String(pumpStateStrWeb) + " </p>"
                  "<br><a href=\"/\">Return to Home Page</a>"
                  "</body></html>"
                 );
  });
  
  server.on("/M", HTTP_GET, [](AsyncWebServerRequest * request) {
    webChangeMode = true;
    //Serial.println("changing mode...");
    //delay(1000);
   request->send(200, "text/html", "<!DOCTYPE HTML><html><head>"
                  "<title>ESP Input Form</title>"
                  "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">"
                  "<style>body{text-align:center;}"
                  "</style>"
                  "</head><body>"
                  "<p>Mode: " + String(ctrlModeStrWeb) + " </p>"
                  "<br><a href=\"/\">Return to Home Page</a>"
                  "</body></html>"
                 );
  });

  // Send a GET request to <ESP_IP>/get?input1=<inputMessage>
  server.on("/get", HTTP_GET, [] (AsyncWebServerRequest * request) {
    // String inputMessage;
    // String inputParam;
    // GET input1 value on <ESP_IP>/get?input1=<inputMessage>
    if (request->hasParam(PARAM_INPUT_1)) {
      inputMessage = request->getParam(PARAM_INPUT_1)->value();
      inputParam = PARAM_INPUT_1;
    }

    // GET input2 value on <ESP_IP>/get?input2=<inputMessage>
    else if (request->hasParam(PARAM_INPUT_2)) {
      inputMessage = request->getParam(PARAM_INPUT_2)->value();
      inputParam = PARAM_INPUT_2;
    }

    else {
      inputMessage = "No message sent";
      inputParam = "none";
    }
    Serial.println(inputMessage);
    request->send(200, "text/html", "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">"
                  "HTTP GET request sent to your ESP on input field ("
                  + inputParam + ") with value: " + inputMessage +
                  "<br><a href=\"/\">Return to Home Page</a>");

  });
  server.onNotFound(notFound);
  server.begin();

  loraSend(node1 + "R");
}

void loraSend(String datasend) {
  LoRa.beginPacket();
  LoRa.print(datasend);
  LoRa.endPacket();
}
void loop() {
  if (digitalRead (sw) == 0) {
    while (digitalRead (sw) == 0);
    delay(200);
    loraSend (node1 + "M");
  }
  if (digitalRead (sw1) == 0) {
    while (digitalRead (sw1) == 0);
    delay(200);
    loraSend (node1 + "P");
  }
  if (digitalRead (sw2) == 0) {
    while (digitalRead (sw2) == 0);
    delay(200);
    loraSend (node1 + "F");
  }

  // try to parse packet
  if (LoRa.parsePacket()) {
    // received a packet
    //Serial.print("Received packet '");
    // read packet
    while (LoRa.available()) {
      if (LoRa.find(ipAddr)) { // ip address this device
        x = LoRa.readString();
        Serial.println(x);            //ส่ง Data ให้ VB
      }
    }
  }
    node = x.substring(0, 2);
    if (node == "X1") {
      temp = x.substring(3, 8).toFloat();
      humi = x.substring(9, 14).toFloat();
      setTemp = x.substring(14, 16).toInt();
      setHumi = x.substring(16, 18).toInt();
      ctrlMode = x.substring(19, 20).toInt();
      pumpState = x.substring(21, 22).toInt();
      fanState = x.substring(23, 24).toInt();

      if (ctrlMode == 0) {
        ctrlModeStr = "AUTO";
        ctrlModeStrWeb = "MANUAL";

      } else {
        ctrlModeStr = "MANUAL";
        ctrlModeStrWeb = "AUTO";
      }
      
      if (pumpState == 0){
        pumpStateStr = "NOT WORK";
        pumpStateStrWeb = "WORKING";
      }else{
        pumpStateStr = "WORKING";
        pumpStateStrWeb = "NOT WORK";
        
      }if (fanState == 0){
        fanStateStr = "NOT WORK";
        fanStateStrWeb = "WORKING";
      }else{
        fanStateStr = "WORKING";
        fanStateStrWeb = "NOT WORK";
    }
  } 
  else {
      Serial.println(node);
    }
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= interval) {
    // save the last time you blinked the LED
    previousMillis = currentMillis;

    loraSend(node1 + "R");

  }
  if(webFan == true){
    loraSend(node1 + "F");
  }
  webFan = false;

  if(webPump == true){
    loraSend(node1 + "P");
  }
  webPump = false;
  
  if (webChangeMode == true) {
    loraSend(node1 + "M");
  }
  webChangeMode = false;

  if (inputParam == "input1") {
    loraSend (node1 + "T" + inputMessage);
  }
  if (inputParam == "input2") {
    loraSend (node1 + "H" + inputMessage);
  }
  inputParam = "";
}
