
// Import required libraries
#include "WiFi.h"
#include "ESPAsyncWebServer.h"

#include "heltec.h"
#define BAND    433E6  //you can set band here directly,e.g. 868E6,915E6

// Replace with your network credentials
const char* ssid = "ASUS";
const char* password = "123456789";

const char* ipAddr = "X0";
String node1 = "X1";
String node;
String x;

float temp ;
float humi;
int setTemp;
int setHumi;
bool ctrlMode;
bool pumpState;
bool fanState;

String inputMessage;
String inputParam;
const char* PARAM_INPUT_1 = "input1"; //temp
const char* PARAM_INPUT_2 = "input2"; //humi

//<link rel="stylesheet" href="https://use.fontawesome.com/releases/v5.7.2/css/all.css" integrity="sha384-fnmOCqbTlWIlj8LyTjo7mOUStjsKC4pOpQbqyi7RrhN7udi9RwhKkMHpvLbHG9Sr" crossorigin="anonymous">

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

String handleTemperature() {
  float t = temp;
  Serial.println(t);
  return String(t);
}

String handleHumidity() {
  float h = humi;
  Serial.println(h);
  return String(h);
}

String handleHumiditylimit() {
  float sh = setHumi;
  Serial.println(sh);
  return String(sh);
}

String handleTemperaturelimit() {
  float st = setTemp;
  Serial.println(st);
  return String(st);
}

String handlePump(){
  String pumpStateStr;
  if(pumpState == 1){
    pumpStateStr = "working";
  }
  else{
    pumpStateStr = "not work";
  }
  Serial.println(pumpStateStr);
  return String(pumpStateStr);
}

String handleFan(){
  String fanStateStr;
  if(fanState == 1){
    fanStateStr = "working";
  }
  else{
    fanStateStr = "not work";
  }
  Serial.println(fanStateStr);
  return String(fanStateStr);
}

String handleMode(){
  String ctrlModeStr;
  if(ctrlMode == 1){
    ctrlModeStr = "manual";
  }
  else{
    ctrlModeStr = "auto";
  }
  Serial.println(ctrlModeStr);
  return String(ctrlModeStr);
}

void loraSend(String datasend) {
  LoRa.beginPacket();
  LoRa.print(datasend);
  LoRa.endPacket();
}

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html>
<head>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <script src="https://kit.fontawesome.com/43df6e3879.js" crossorigin="anonymous"></script>
  <style>
    html {
     font-family: Arial;
     display: inline-block;
     margin: 0px auto;
     text-align: center;
    }
    h2 { font-size: 3.0rem; }
    .text { font-size: 3.0rem; padding-bottom: 10px; }
    .units { font-size: 1.2rem; }
    .labels{
      font-size: 1.5rem;
      vertical-align:middle;
    }
    .button {
      background-color: #4CAF50;border: none;color: white;padding: 12px 32px;
      text-align: center;text-decoration: none;display: inline-block;
      font-size: 16px; margin: 4px 2px; cursor: pointer;
    }
    .button2 {background-color: #008CBA;}
    .button3 {background-color: #f44336;}
    .button4 {background-color: #e7e7e7; color: black;}
    .button5 {background-color: #555555;}
    
  </style>
</head>
<body>
  <h2>Mushroom Web Server</h2>
  <div class="text">
    <i class="fas fa-thermometer-half" style="color:#059e8a;"></i> 
    <span class="labels">Temperature</span> 
    <span id="temperature">%TEMPERATURE%</span>
    <sup class="units" style="font-weight:bold; font-size:1.5rem;">&deg;C</sup>
    <span class="labels">Temp Limit</span>
    <span id="temperaturelimit">%TEMPERATURELIMIT%</span>
    <sup class="units" style="font-weight:bold; font-size:1.5rem;">&deg;C</sup>
  </div>

   <div class="text">
    <i class="fas fa-tint" style="color:#00add6;"></i>  
    <span class="labels">Humidity</span> 
    <span id="humidity">%HUMIDITY%</span>
    <sup class="units"><i class="fas fa-percent"></i></sup>
    <span class="labels">Humi Limit</span>
    <span id="humiditylimit">%HUMIDITYLIMIT%</span>
    <sup class="units"><i class="fas fa-percent"></i></sup>
   </div>

   <div class="text">
    <i class="fas fa-shower" style="color:#00add6;"></i>
    <span class="labels">Pump</span>
    <span id="pump">%PUMP%</span>
   </div>

   <div class="text">
    <i class="fas fa-fan" style="color:#00add6;"></i>
    <span class="labels">Fan</span>
    <span id="fan">%FAN%</span>
   </div>

   <div class="text">
    <i class="fas fa-sliders-h" style="color:#00add6;"></i>
    <span class="labels">Mode</span>
    <span id="mode">%MODE%</span>
   </div>

   <div>
    <a href="/P" class = "button button2"><i class="fas fa-shower"></i>pump on/off</a>
    <a href="/F" class = "button button2"><i class="fas fa-fan"></i>fan on/off</a>
    <a href="/M" class = "button "><i class="fas fa-sliders-h"></i>change mode</a>
    <a href="/setting" class = "button "><i class="fas fa-cog"></i>setting</a>
   </div>
   
</body>
<script>
setInterval(function ( ) {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("temperature").innerHTML = this.responseText;
    }
  };
  xhttp.open("GET", "/temperature", true);
  xhttp.send();
}, 5000 ) ;

setInterval(function ( ) {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("temperaturelimit").innerHTML = this.responseText;
    }
  };
  xhttp.open("GET", "/temperaturelimit", true);
  xhttp.send();
}, 5000 ) ;

setInterval(function ( ) {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("humidity").innerHTML = this.responseText;
    }
  };
  xhttp.open("GET", "/humidity", true);
  xhttp.send();
}, 5000 ) ;

setInterval(function ( ) {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("humiditylimit").innerHTML = this.responseText;
    }
  };
  xhttp.open("GET", "/humiditylimit", true);
  xhttp.send();
}, 5000 ) ;

setInterval(function ( ) {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("pump").innerHTML = this.responseText;
    }
  };
  xhttp.open("GET", "/pump", true);
  xhttp.send();
}, 5000 ) ;

setInterval(function ( ) {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("fan").innerHTML = this.responseText;
    }
  };
  xhttp.open("GET", "/fan", true);
  xhttp.send();
}, 5000 ) ;

setInterval(function ( ) {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("mode").innerHTML = this.responseText;
    }
  };
  xhttp.open("GET", "/mode", true);
  xhttp.send();
}, 5000 ) ;

</script>
</html>)rawliteral";

// Replaces placeholder with DHT values
String processor(const String& var) {
  //Serial.println(var);
  if (var == "TEMPERATURE") {
    return handleTemperature();
  }
  else if (var == "HUMIDITY") {
    return handleHumidity();
  }
  else if (var == "HUMIDITYLIMIT") {
    return handleHumiditylimit();
  }
  else if (var == "TEMPERATURELIMIT") {
    return handleTemperaturelimit();
  }
  else if (var == "PUMP") {
    return handlePump();
  }
  else if (var == "FAN") {
    return handleFan();
  }
  else if (var == "MODE") {
    return handleMode();
  }
  return String();
}

void setup() {
  // Serial port for debugging purposes
  Serial.begin(115200);
  Heltec.begin(false /*DisplayEnable Enable*/, true /*Heltec.LoRa Disable*/, true /*Serial Enable*/, true /*PABOOST Enable*/, BAND /*long BAND*/);

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }

  // Print ESP32 Local IP Address
  Serial.println(WiFi.localIP());

  // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send_P(200, "text/html", index_html, processor);
  });
  server.on("/temperature", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send_P(200, "text/plain", handleTemperature().c_str());
  });
  server.on("/humidity", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send_P(200, "text/plain", handleHumidity().c_str());
  });
  server.on("/humiditylimit", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send_P(200, "text/plain", handleHumiditylimit().c_str());
  });
  server.on("/temperaturelimit", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send_P(200, "text/plain", handleTemperaturelimit().c_str());
  });
  server.on("/pump", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send_P(200, "text/plain", handlePump().c_str());
  });
  server.on("/fan", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send_P(200, "text/plain", handleFan().c_str());
  });
  server.on("/mode", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send_P(200, "text/plain", handleMode().c_str());
  });

  server.on("/M", HTTP_GET, [](AsyncWebServerRequest * request) {
    loraSend(node1 + "M");
    request->send(200, "text/html", "<!DOCTYPE HTML><html><head>"
                  "<title>ESP Input Form</title>"
                  "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">"
                  "<style>body{text-align:center;}"
                  "</style>"
                  "</head><body>"
                  "<p>sending command via lora to mushroom node...</p>"
                  "<br><a href=\"/\">Return to Home Page</a>"
                  "</body></html>"
                 );
  });

  server.on("/P", HTTP_GET, [](AsyncWebServerRequest * request) {
    loraSend(node1 + "P");
    request->send(200, "text/html", "<!DOCTYPE HTML><html><head>"
                  "<title>ESP Input Form</title>"
                  "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">"
                  "<style>body{text-align:center;}"
                  "</style>"
                  "</head><body>"
                  "<p>sending command via lora to mushroom node...</p>"
                  "<br><a href=\"/\">Return to Home Page</a>"
                  "</body></html>"
                 );
  });

  server.on("/setting", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send(200, "text/html", "<!DOCTYPE HTML><html><head>"
                  "<title>ESP Input Form</title>"
                  "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">"
                  "<style>body{text-align:center;}"
                  "</style>"
                  "</head><body>"
                  "<p>setting Temperature and Humidity Limit...</p>"
                  "<form action=\"/get\">"
                  "Temp Limit: <input type=\"number\" name=\"input1\">"
                  "<input type=\"submit\" value=\"Submit\">"
                  "</form><br>"
                  "<form action=\"/get\">"
                  "Humi Limit: <input type=\"number\" name=\"input2\">"
                  "<input type=\"submit\" value=\"Submit\">"
                  "</form>"
                  "<br><a href=\"/\">Return to Home Page</a>"
                  "</body></html>"
                 );
  });

  server.on("/F", HTTP_GET, [](AsyncWebServerRequest * request) {
    loraSend(node1 + "F");
    request->send(200, "text/html", "<!DOCTYPE HTML><html><head>"
                  "<title>ESP Input Form</title>"
                  "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">"
                  "<style>body{text-align:center;}"
                  "</style>"
                  "</head><body>"
                  "<p>sending command via lora to mushroom node...</p>"
                  "<br><a href=\"/\">Return to Home Page</a>"
                  "</body></html>"
                 );
  });

  // Send a GET request to <ESP_IP>/get?input1=<inputMessage>
  server.on("/get", HTTP_GET, [] (AsyncWebServerRequest * request) {
    
    // GET input1 value on <ESP_IP>/get?input1=<inputMessage>
    if (request->hasParam(PARAM_INPUT_1)) {
      inputMessage = request->getParam(PARAM_INPUT_1)->value();
      inputParam = PARAM_INPUT_1;
      loraSend(node1 + "T" + inputMessage);
    }

    // GET input2 value on <ESP_IP>/get?input2=<inputMessage>
    else if (request->hasParam(PARAM_INPUT_2)) {
      inputMessage = request->getParam(PARAM_INPUT_2)->value();
      inputParam = PARAM_INPUT_2;
      loraSend(node1 + "H" + inputMessage);
    }

    else {
      inputMessage = "No message sent";
      inputParam = "none";
    }
    Serial.println(inputMessage);
    request->send(200, "text/html", "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">"
                  "HTTP GET request sent to your ESP on input field ("
                  + inputParam + ") with value: " + inputMessage +
                  "<br><a href=\"/\">Return to Home Page</a>"
                  "<br><a href=\"/setting\">Return to setting page</a>");

  });

  // Start server
  server.begin();

  loraSend(node1 + "R");
}


void loop(void) {

  // try to parse packet
  if (LoRa.parsePacket()) {
    // received a packet
    //Serial.print("Received packet '");
    // read packet
    while (LoRa.available()) {
      if (LoRa.find(ipAddr)) { // ip address this device "
        x = LoRa.readString();
        Serial.println(x);
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

  }


  if (millis() % 5000 == 0) {
    loraSend(node1 + "R");
  } // Display a serial diagnostic print to check its running

  delay(1);
}
