// Import required libraries
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <HTTPClient.h>
#include "ESPAsyncWebServer.h"
#include "heltec.h"
#include "time.h"
#include <string.h>
#include <Arduino_JSON.h>

#define BAND 433E6 //you can set band here directly,e.g. 868E6,915E6
#define led_check_wifi_status 2
#include "config.h"

// variables for get ngrok public url
String json;
String tunnelsNameArr[2];
String publicURLArr[2];
String database_server_url, web_server_url;

const char *ipAddr = "X0";
String node1 = "X1";
String node;
String x;
String dataForVB;

float temp;
float humi;
int set_temp_min, set_temp_max;
int set_humi_min, set_humi_max;
bool ctrlMode;
bool pumpState;
bool fanState;

String line = "";

String inputMessage;
String inputParam;
bool lora_ctrl_pump, lora_ctrl_fan, lora_ctrl_mode, lora_request_update;

const char *PARAM_INPUT_1 = "input1"; //temp Max
const char *PARAM_INPUT_3 = "input3"; //temp Min
const char *PARAM_INPUT_2 = "input2"; //humi Min
const char *PARAM_INPUT_4 = "input4"; //humi Max

//const char *_PARAM_INPUT_1 = "output";
//const char *_PARAM_INPUT_2 = "state";
//Timer-------------------------------------------------------------------------------
unsigned long previousMillis_request = 0;
unsigned long previousMillis_vb = 0;
//unsigned long previousMillis_db = 0;
const int interval_request = 60000; //Interval for request data via lora
const int interval_vb = 1000;       //Interval for print data to VB(Windows App)
//const unsigned long interval_db = 40000;  //Interval for insert data into database server

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);
HTTPClient http;

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html>
<head>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <title>Mushroom Web Server</title>
  <script src="https://kit.fontawesome.com/43df6e3879.js" crossorigin="anonymous"></script>
     <style>
        html {
            font-family: Arial, Helvetica, sans-serif;
            display: inline-block;
            margin: 0px auto;
            text-align: center;
        }

        h2 {
            font-size: 3.0rem;
        }

        .text {
            font-size: 3.0rem;
            padding-bottom: 10px;
        }

        .text2 {
            font-size: 2.0rem;
            padding-bottom: 10px;
        }

        .units {
            font-size: 1.2rem;
        }

        .labels {
            font-size: 1.5rem;
            vertical-align: middle;
        }

        .button {
            background-color: #4CAF50;
            border: none;
            color: white;
            padding: 12px 32px;
            text-align: center;
            text-decoration: none;
            display: inline-block;
            font-size: 16px;
            margin: 4px 2px;
            cursor: pointer;
        }

        .button2 {
            background-color: white; 
            color: black; 
            border: 2px solid #008CBA;
            border-radius: 12px;
        }

        .button2:hover {
            background-color: #008CBA;
            color: white;
        }

        .button3 {
            background-color: white; 
            color: black; 
            border: 2px solid #f44336;
            border-radius: 12px;
        }

        .button3:hover {
            background-color: #f44336;
            color: white;
        }

        .sidenav {
            height: 100%;
            width: 0;
            position: fixed;
            z-index: 1;
            top: 0;
            left: 0;
            background-color: #111;
            overflow-x: hidden;
            transition: 0.5s;
            padding-top: 60px;
        }

        .sidenav a {
            padding: 8px 8px 8px 32px;
            text-decoration: none;
            font-size: 25px;
            color: #818181;
            display: block;
            transition: 0.3s;
        }

        .sidenav a:hover {
            color: #f1f1f1;
        }

        .sidenav .closebtn {
            position: absolute;
            top: 0;
            right: 25px;
            font-size: 36px;
            margin-left: 50px;
        }
        
        @media screen and (max-height: 450px) {
            .sidenav {
                padding-top: 15px;
            }

            .sidenav a {
                font-size: 18px;
            }
        }
        .switch {position: relative; display: inline-block; width: 120px; height: 68px} 
        .switch input {display: none}
        .slider {position: absolute; top: 0; left: 0; right: 0; bottom: 0; background-color: #ccc; border-radius: 6px}
        .slider:before {position: absolute; content: ""; height: 52px; width: 52px; left: 8px; bottom: 8px; background-color: #fff; -webkit-transition: .4s; transition: .4s; border-radius: 3px}
        input:checked+.slider {background-color: #b30000}
        input:checked+.slider:before {-webkit-transform: translateX(52px); -ms-transform: translateX(52px); transform: translateX(52px)}
  
    </style>
</head>
<script>

    function openNav() {
        document.getElementById("mySidenav").style.width = "250px";
    }

    function closeNav() {
        document.getElementById("mySidenav").style.width = "0";
    }
    function logoutButton() {
        var xhr = new XMLHttpRequest();
        xhr.open("GET", "/logout", true);
        xhr.send();
        setTimeout(function () { window.open("/logged-out", "_self"); }, 1000);
    }

    function getmode() {
        var xhttp = new XMLHttpRequest();
          xhttp.onreadystatechange = function () {
            if (this.readyState == 4 && this.status == 200) {
                alert(this.responseText);
            }
        };
          xhttp.open("GET", "/M", true);
          xhttp.send();
    }

    function requestupdate() {
        var xhttp = new XMLHttpRequest();
          xhttp.onreadystatechange = function () {
            if (this.readyState == 4 && this.status == 200) {
                alert(this.responseText);
            }
        };
          xhttp.open("GET", "/requestupdate", true);
          xhttp.send();
    }
    
    
    function toggleCheckbox_pump(element) {
      var xhr = new XMLHttpRequest();
          xhr.onreadystatechange = function () {
            if (this.readyState == 4 && this.status == 200) {
                alert(this.responseText);
            }
        };
        if(element.checked){ xhr.open("GET", "/P", true); }
        else { xhr.open("GET", "/P", true); }
        xhr.send();
    }

    function toggleCheckbox_fan(element) {
      var xhr = new XMLHttpRequest();
          xhr.onreadystatechange = function () {
            if (this.readyState == 4 && this.status == 200) {
                alert(this.responseText);
            }
        };
        if(element.checked){ xhr.open("GET", "/F", true); }
        else { xhr.open("GET", "/F", true); }
        xhr.send();
    }
    
    setInterval(function () {
        var xhttp = new XMLHttpRequest();
        xhttp.onreadystatechange = function () {
            if (this.readyState == 4 && this.status == 200) {
                document.getElementById("temperature").innerHTML = this.responseText;
            }
        };
        xhttp.open("GET", "/temperature", true);
        xhttp.send();
    }, 5000);
    
    setInterval(function () {
        var xhttp = new XMLHttpRequest();
        xhttp.onreadystatechange = function () {
            if (this.readyState == 4 && this.status == 200) {
                document.getElementById("ctrlbtn_pump").innerHTML = this.responseText;
            }
        };
        xhttp.open("GET", "/ctrlbtn_pump", true);
        xhttp.send();
    }, 5000);

    setInterval(function () {
        var xhttp = new XMLHttpRequest();
        xhttp.onreadystatechange = function () {
            if (this.readyState == 4 && this.status == 200) {
                document.getElementById("ctrlbtn_fan").innerHTML = this.responseText;
            }
        };
        xhttp.open("GET", "/ctrlbtn_fan", true);
        xhttp.send();
    }, 5000);
    
    setInterval(function () {
        var xhttp = new XMLHttpRequest();
        xhttp.onreadystatechange = function () {
            if (this.readyState == 4 && this.status == 200) {
                document.getElementById("temperaturelimitmin").innerHTML = this.responseText;
            }
        };
        xhttp.open("GET", "/temperaturelimitmin", true);
        xhttp.send();
    }, 5000);

    setInterval(function () {
        var xhttp = new XMLHttpRequest();
        xhttp.onreadystatechange = function () {
            if (this.readyState == 4 && this.status == 200) {
                document.getElementById("temperaturelimitmax").innerHTML = this.responseText;
            }
        };
        xhttp.open("GET", "/temperaturelimitmax", true);
        xhttp.send();
    }, 5000);

    setInterval(function () {
        var xhttp = new XMLHttpRequest();
        xhttp.onreadystatechange = function () {
            if (this.readyState == 4 && this.status == 200) {
                document.getElementById("humidity").innerHTML = this.responseText;
            }
        };
        xhttp.open("GET", "/humidity", true);
        xhttp.send();
    }, 5000);

    setInterval(function () {
        var xhttp = new XMLHttpRequest();
        xhttp.onreadystatechange = function () {
            if (this.readyState == 4 && this.status == 200) {
                document.getElementById("humiditylimitmin").innerHTML = this.responseText;
            }
        };
        xhttp.open("GET", "/humiditylimitmin", true);
        xhttp.send();
    }, 5000);

    setInterval(function () {
        var xhttp = new XMLHttpRequest();
        xhttp.onreadystatechange = function () {
            if (this.readyState == 4 && this.status == 200) {
                document.getElementById("humiditylimitmax").innerHTML = this.responseText;
            }
        };
        xhttp.open("GET", "/humiditylimitmax", true);
        xhttp.send();
    }, 5000);
    
    setInterval(function () {
        var xhttp = new XMLHttpRequest();
        xhttp.onreadystatechange = function () {
            if (this.readyState == 4 && this.status == 200) {
                document.getElementById("pump").innerHTML = this.responseText;
            }
        };
        xhttp.open("GET", "/pump", true);
        xhttp.send();
    }, 5000);

    setInterval(function () {
        var xhttp = new XMLHttpRequest();
        xhttp.onreadystatechange = function () {
            if (this.readyState == 4 && this.status == 200) {
                document.getElementById("fan").innerHTML = this.responseText;
            }
        };
        xhttp.open("GET", "/fan", true);
        xhttp.send();
    }, 5000);

    setInterval(function () {
        var xhttp = new XMLHttpRequest();
        xhttp.onreadystatechange = function () {
            if (this.readyState == 4 && this.status == 200) {
                document.getElementById("mode").innerHTML = this.responseText;
            }
        };
        xhttp.open("GET", "/mode", true);
        xhttp.send();
    }, 5000);
     
</script>
</script>
<body>
    <script language="javascript" type="text/javascript">

        openDataLogger = function() {
          var xhttp = new XMLHttpRequest();
          xhttp.onreadystatechange = function () {
            if (this.readyState == 4 && this.status == 200) {
                location.href = this.responseText;
            }
        };
          xhttp.open("GET", "/datalogger", true);
          xhttp.send();
        }
        openChart = function() {
          var xhttp = new XMLHttpRequest();
          xhttp.onreadystatechange = function () {
            if (this.readyState == 4 && this.status == 200) {
                location.href = this.responseText;
            }
        };
          xhttp.open("GET", "/chart", true);
          xhttp.send();
        }
        openPhpMyAdmin = function() {
          var xhttp = new XMLHttpRequest();
          xhttp.onreadystatechange = function () {
            if (this.readyState == 4 && this.status == 200) {
                location.href = this.responseText;
            }
        };
          xhttp.open("GET", "/phpmyadmin", true);
          xhttp.send();
        }

    </script>

    <div id="mySidenav" class="sidenav" style="text-align: left;">
        <a href="javascript:void(0)" class="closebtn" onclick="closeNav()">&times;</a>
        <a href="javascript:openDataLogger()"><i class="fa fa-table" aria-hidden="true"></i>&nbsp;Data Logger</a>
        <a href="javascript:openChart()"><i class="fa fa-line-chart" aria-hidden="true"></i>&nbsp;Chart</a>
        <a href="javascript:openPhpMyAdmin()"><i class="fa fa-database" aria-hidden="true"></i>&nbsp;phpMyAdmin</a>
        <a href="/setting"><i class="fas fa-cog"></i>&nbsp;Setting</a>
        <a href="#" onclick="logoutButton()"><i class="fa fa-sign-out" aria-hidden="true"></i>&nbsp;Logout</a>
    </div>
    <div style="text-align: left;">
        <span style="font-size:30px;cursor:pointer" onclick="openNav()">&#9776;</span>
    </div>

    <h2 style="margin-top: 0px;">Mushroom Web Server</h2>
    <div class="text">
        <i class="fas fa-thermometer-half" style="color:#059e8a;"></i>
        <span class="labels">Temperature</span>
        <span id="temperature">%TEMPERATURE%</span>
        <sup class="units" style="font-weight:bold; font-size:1.5rem;">&deg;C</sup>
        
        <br><span class="labels">Min</span>
        <span id="temperaturelimitmin" class="text2">%TEMPERATURELIMITMIN%</span>
        <sup class="units" style="font-weight:bold; font-size:1.5rem;">&deg;C</sup>
        
        <span class="labels">Max</span>
        <span id="temperaturelimitmax" class="text2">%TEMPERATURELIMITMAX%</span>
        <sup class="units" style="font-weight:bold; font-size:1.5rem;">&deg;C</sup>
    </div>

    <div class="text" style="padding-top: 25px;">
        <i class="fas fa-tint" style="color:#00add6;"></i>
        <span class="labels">Humidity</span>
        <span id="humidity">%HUMIDITY%</span>
        <sup class="units"><i class="fas fa-percent"></i></sup>
        
        <br><span class="labels">Min</span>
        <span id="humiditylimitmin" class="text2">%HUMIDITYLIMITMIN%</span>
        <sup class="units"><i class="fas fa-percent"></i></sup>
        
        <span class="labels">Max</span>
        <span id="humiditylimitmax" class="text2">%HUMIDITYLIMITMAX%</span>
        <sup class="units"><i class="fas fa-percent"></i></sup>
    </div>

    <div class="text" style="padding-top: 25px;">
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
        <a id="ctrlbtn_pump">%BUTTONPLACEHOLDERPUMP%</a>
        <a id="ctrlbtn_fan">%BUTTONPLACEHOLDERFAN%</a>
    </div>
    
    <div style="padding-top: 20px; padding-bottom: 20px;">
        <a href="#" onclick="getmode()" class="button button3"><i class="fas fa-sliders-h"></i>&nbsp;change mode</a>
        <a href="#" onclick="requestupdate()" class="button button2"><i class="fa fa-refresh" aria-hidden="true"></i>&nbsp;request update</a>
    </div>
   
</body>

</html>)rawliteral";

const char logout_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html>
<head>
  <meta name="viewport" content="width=device-width, initial-scale=1">
</head>
<body>
  <p>Logged out or <a href="/">return to homepage</a>.</p>
  <p><strong>Note:</strong> close all web browser tabs to complete the logout process.</p>
</body>
</html>
)rawliteral";

const char setting_html[] PROGMEM = R"rawliteral(
  <!DOCTYPE HTML>
<html>

<head>
    <title>ESP Input Form</title>
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <style>
        body {
            text-align: center;
        }
    </style>
</head>

<body>
    <p>Setting Temperature and Humidity Limit...</p>
    <form action="/get">
        Temp Limit Min: <input type="number" name="input3" min="10" max="40">
        <input type="submit" value="Submit">
    </form><br>
    <form action="/get">
        Temp Limit Max: <input type="number" name="input1" min="10" max="40">
        <input type="submit" value="Submit">
    </form><br>
    <form action="/get">
        Humi Limit Min: <input type="number" name="input2" min="50" max="99">
        <input type="submit" value="Submit">
    </form>
    <br>
    <form action="/get">
        Humi Limit Max: <input type="number" name="input4" min="50" max="99">
        <input type="submit" value="Submit">
    </form>
    <br><a href="/">Return to Home Page</a>
    
</body>

</html>
)rawliteral";

String handleTemperature()
{
  float t = temp;
  //Serial.println(t);
  return String(t);
}

String handleHumidity()
{
  float h = humi;
  //Serial.println(h);
  return String(h);
}
//-------------------------------------------------edit
String handleHumiditylimitMin()
{
  float sh = set_humi_min;
  //Serial.println(sh);
  return String(sh);
}
String handleHumiditylimitMax()
{
  float sh = set_humi_max;
  //Serial.println(sh);
  return String(sh);
}
String handleTemperaturelimitMin()
{
  float st = set_temp_min;
  //Serial.println(st);
  return String(st);
}
String handleTemperaturelimitMax()
{
  float st = set_temp_max;
  //Serial.println(st);
  return String(st);
}
//----------------------------------------------------
String handlePump()
{
  String pumpStateStr;
  if (pumpState == 1)
  {
    pumpStateStr = "ON";
  }
  else
  {
    pumpStateStr = "OFF";
  }
  //Serial.println(pumpStateStr);
  return String(pumpStateStr);
}

String handleFan()
{
  String fanStateStr;
  if (fanState == 1)
  {
    fanStateStr = "ON";
  }
  else
  {
    fanStateStr = "OFF";
  }
  //Serial.println(fanStateStr);
  return String(fanStateStr);
}

String handleMode()
{
  String ctrlModeStr;
  if (ctrlMode == 1)
  {
    ctrlModeStr = "MANUAL";
  }
  else
  {
    ctrlModeStr = "AUTO";
  }
  //Serial.println(ctrlModeStr);
  return String(ctrlModeStr);
}

void loraSend(String datasend)
{
  LoRa.beginPacket();
  LoRa.print(datasend);
  LoRa.endPacket();
}
String outputState(int output){
  if(output){
    return "checked";
  }
  else {
    return "";
  }
}

String ctrlbtn_pump()
{
    if(ctrlMode){
      String buttons = "";
      buttons += "<h4>Control - PUMP&nbsp;<i class=\"fas fa-shower\"></i></h4><label class=\"switch\"><input type=\"checkbox\" onchange=\"toggleCheckbox_pump(this)\" id=\"33\" " + outputState(pumpState) + "><span class=\"slider\"></span></label>";
      return buttons;
    }
    else{
      String txt = "";
      txt += "<h4></h4>";
      return txt;
    }
}

String ctrlbtn_fan()
{
    if(ctrlMode){
      String buttons = "";
      buttons += "<h4>Control - FAN&nbsp;<i class=\"fas fa-fan\"></i></h4><label class=\"switch\"><input type=\"checkbox\" onchange=\"toggleCheckbox_fan(this)\" id=\"4\" " + outputState(fanState) + "><span class=\"slider\"></span></label>";
      return buttons;
    }
    else{
      String txt = "";
      txt += "<h4></h4>";
      return txt;
    }
}

// Replaces placeholder with DHT values
String processor(const String &var)
{
  //Serial.println(var);
  if (var == "TEMPERATURE")
  {
    return handleTemperature();
  }
  else if (var == "HUMIDITY")
  {
    return handleHumidity();
  }
  else if (var == "HUMIDITYLIMITMIN")
  {
    return handleHumiditylimitMin();
  }
  else if (var == "HUMIDITYLIMITMAX")
  {
    return handleHumiditylimitMax();
  }
  else if (var == "TEMPERATURELIMITMIN")
  {
    return handleTemperaturelimitMin();
  }
  else if (var == "TEMPERATURELIMITMAX")
  {
    return handleTemperaturelimitMax();
  }
  else if (var == "PUMP")
  {
    return handlePump();
  }
  else if (var == "FAN")
  {
    return handleFan();
  }
  else if (var == "MODE")
  {
    return handleMode();
  }
  else if(var == "BUTTONPLACEHOLDERPUMP"){
    return ctrlbtn_pump();
  }
  else if(var == "BUTTONPLACEHOLDERFAN"){
    return ctrlbtn_fan();
  }
  return String();
}

void NotifyLine(String t)
{
  WiFiClientSecure client;
  if (!client.connect("notify-api.line.me", 443))
  {
    Serial.println("Connection failed -LINE Notify");

    return;
  }
  String req = "";
  req += "POST /api/notify HTTP/1.1\r\n";
  req += "Host: notify-api.line.me\r\n";
  req += "Authorization: Bearer " + String(TokenLine) + "\r\n";
  req += "Cache-Control: no-cache\r\n";
  req += "User-Agent: ESP32\r\n";
  req += "Content-Type: application/x-www-form-urlencoded\r\n";
  req += "Content-Length: " + String(String("message=" + t).length()) + "\r\n";
  req += "\r\n";
  req += "message=" + t;
  //Serial.println(req);
  client.print(req);
  delay(20);
  //Serial.println("-------------");
  while (client.connected())
  {
    String line = client.readStringUntil('\n');
    if (line == "\r")
    {
      break;
    }
  }
}

void insertDB()
{
  // Your Domain name with URL path or IP address with path
  const String serverName = database_server_url + "/post-esp-data.php";
  http.begin(serverName);

  // Specify content-type header
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");

  String ctrlModeStr, pumpStateStr, fanStateStr;
  if (ctrlMode == 1){
    ctrlModeStr = "MANUAL";
  }else{
    ctrlModeStr = "AUTO";
  }if (pumpState == 1){
    pumpStateStr = "ON";
  }else{
    pumpStateStr = "OFF";
  }if (fanState == 1){
    fanStateStr = "ON";
  }else{
    fanStateStr = "OFF";
  }
  // Prepare your HTTP POST request data
  String httpRequestData = "api_key=" + apiKeyValue + "&temp=" + String(temp) + "&humi=" + String(humi) + "&temp_limit_min=" + String(set_temp_min) + "&temp_limit_max=" + String(set_temp_max) + "&humi_limit_min=" + String(set_humi_min) + "&humi_limit_max=" + String(set_humi_max) + "&ctrl_mode=" + ctrlModeStr + "&pump_state=" + pumpStateStr + "&fan_state=" + fanStateStr + "";

  // Send HTTP POST request
  int httpResponseCode = http.POST(httpRequestData);

  NotifyLine("Saving Data into Database.\nHTTP Response code: " + String(httpResponseCode));
  // Free resources
  http.end();
}

String httpGETRequest(const char *serverName)
{

  // Your IP address with path or Domain name with URL path
  http.begin(serverName);

  // Send HTTP POST request
  int httpResponseCode = http.GET();

  String payload = "{}";

  if (httpResponseCode > 0)
  {
    //Serial.print("HTTP Response code: ");
    //Serial.println(httpResponseCode);
    payload = http.getString();
  }
  else
  {
    //Serial.print("Error code: ");
    //Serial.println(httpResponseCode);
  }
  // Free resources
  http.end();

  return payload;
}


String dataLogger()
{
  String url = database_server_url + "/esp-data.php";
  return String(url);
}
String chart()
{
  String url = database_server_url + "/esp-chart.php";
  return String(url);
}
String phpMyAdmin()
{
  String url = database_server_url + "/phpmyadmin";
  return String(url);
} 

void setup()
{
  // Serial port for debugging purposes
  Serial.begin(115200);
  Heltec.begin(false /*DisplayEnable Enable*/, true /*Heltec.LoRa Disable*/, true /*Serial Enable*/, true /*PABOOST Enable*/, BAND /*long BAND*/);
  pinMode(led_check_wifi_status, OUTPUT);
  digitalWrite(led_check_wifi_status, LOW);
  // Configures static IP address
  if (!WiFi.config(local_IP, gateway, subnet, primaryDNS, secondaryDNS)) {
    Serial.println("STA Failed to configure");
  }
  // Connect to Wi-Fi
  WiFi.begin(ssid, password);

  int count = 0;
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.println("Connecting to WiFi..");
    delay(1000);
    count++;
    if (count == 30) {
      count = 0;
      break;
    }
  }

  if(WiFi.status() == WL_CONNECTED){
    // Print ESP32 Local IP Address
    Serial.print("Connected to " + String(ssid) + " ");
    Serial.println(WiFi.localIP());
    //If wifi is connected led_check_wifi_status will HIGH
    digitalWrite(led_check_wifi_status, HIGH);
  }
  
  //-------------------------------------------------------------------------------------
  //Get ngrok public url
  //Check WiFi connection status
  if (WiFi.status() == WL_CONNECTED)
  {

    json = httpGETRequest(serverName);
    Serial.println(json);
    JSONVar myObject = JSON.parse(json);

    // JSON.typeof(jsonVar) can be used to get the type of the var
    if (JSON.typeof(myObject) == "undefined")
    {
      Serial.println("Parsing input failed!");
      return;
    }

    Serial.print("JSON object = ");
    Serial.println(myObject);

    // myObject.keys() can be used to get an array of all the keys in the object
    JSONVar keys = myObject.keys();

    for (int i = 0; i < keys.length(); i++)
    {
      JSONVar value = myObject[keys[i]];
      //Serial.print(keys[i]);
      //Serial.print(" = ");
      //Serial.println(value);
      tunnelsNameArr[i] = keys[i];
      publicURLArr[i] = value;
    }
    Serial.print(tunnelsNameArr[0] + " = ");
    Serial.println(publicURLArr[0]);
    Serial.print(tunnelsNameArr[1] + " = ");
    Serial.println(publicURLArr[1]);
  }
  else
  {
    Serial.println("WiFi Disconnected -Get ngrok public url");
  }

  if (tunnelsNameArr[0] == "web_server")
  {
    web_server_url = publicURLArr[0];
    database_server_url = publicURLArr[1];
  }
  else
  {
    web_server_url = publicURLArr[1];
    database_server_url = publicURLArr[0];
  }

  Serial.println(web_server_url);
  Serial.println(database_server_url);
  //------------------------------------------------------------------
  //LINE Notify
  String line_notify_text = "";
  line_notify_text += "Started Mushroom Web Server...\n";
  line_notify_text += "Local URL : " + WiFi.localIP().toString();
  line_notify_text += "\nPublic URL : " + web_server_url;
  line_notify_text += "\nDatabase Server...\nURL : " + database_server_url;
  NotifyLine(line_notify_text);
  //--------------------------------------------------------------------
  // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    if (!request->authenticate(http_username, http_password))
      return request->requestAuthentication();
    request->send_P(200, "text/html", index_html, processor);
  });
  server.on("/ctrlbtn_pump", HTTP_GET, [](AsyncWebServerRequest *request) {
    if (!request->authenticate(http_username, http_password))
      return request->requestAuthentication();
    request->send_P(200, "text/plain", ctrlbtn_pump().c_str());
  });
  server.on("/ctrlbtn_fan", HTTP_GET, [](AsyncWebServerRequest *request) {
    if (!request->authenticate(http_username, http_password))
      return request->requestAuthentication();
    request->send_P(200, "text/plain", ctrlbtn_fan().c_str());
  });
  server.on("/temperature", HTTP_GET, [](AsyncWebServerRequest *request) {
    if (!request->authenticate(http_username, http_password))
      return request->requestAuthentication();
    request->send_P(200, "text/plain", handleTemperature().c_str());
  });
  server.on("/humidity", HTTP_GET, [](AsyncWebServerRequest *request) {
    if (!request->authenticate(http_username, http_password))
      return request->requestAuthentication();
    request->send_P(200, "text/plain", handleHumidity().c_str());
  });
  //------------------------------------------------------------------------------edit
  server.on("/humiditylimitmin", HTTP_GET, [](AsyncWebServerRequest *request) {
    if (!request->authenticate(http_username, http_password))
      return request->requestAuthentication();
    request->send_P(200, "text/plain", handleHumiditylimitMin().c_str());
  });
  server.on("/humiditylimitmax", HTTP_GET, [](AsyncWebServerRequest *request) {
    if (!request->authenticate(http_username, http_password))
      return request->requestAuthentication();
    request->send_P(200, "text/plain", handleHumiditylimitMax().c_str());
  });
  server.on("/temperaturelimitmin", HTTP_GET, [](AsyncWebServerRequest *request) {
    if (!request->authenticate(http_username, http_password))
      return request->requestAuthentication();
    request->send_P(200, "text/plain", handleTemperaturelimitMin().c_str());
  });
  server.on("/temperaturelimitmax", HTTP_GET, [](AsyncWebServerRequest *request) {
    if (!request->authenticate(http_username, http_password))
      return request->requestAuthentication();
    request->send_P(200, "text/plain", handleTemperaturelimitMax().c_str());
  });
  //-------------------------------------------------------------------------------
  server.on("/pump", HTTP_GET, [](AsyncWebServerRequest *request) {
    if (!request->authenticate(http_username, http_password))
      return request->requestAuthentication();
    request->send_P(200, "text/plain", handlePump().c_str());
  });
  server.on("/fan", HTTP_GET, [](AsyncWebServerRequest *request) {
    if (!request->authenticate(http_username, http_password))
      return request->requestAuthentication();
    request->send_P(200, "text/plain", handleFan().c_str());
  });
  server.on("/mode", HTTP_GET, [](AsyncWebServerRequest *request) {
    if (!request->authenticate(http_username, http_password))
      return request->requestAuthentication();
    request->send_P(200, "text/plain", handleMode().c_str());
  });
  //-------------------------------------------------------------------
  //Send public url for access datalogger chart phpMyAdmin
  server.on("/datalogger", HTTP_GET, [](AsyncWebServerRequest *request) {
    if (!request->authenticate(http_username, http_password))
      return request->requestAuthentication();
    request->send_P(200, "text/plain", dataLogger().c_str());
  });
  server.on("/chart", HTTP_GET, [](AsyncWebServerRequest *request) {
    if (!request->authenticate(http_username, http_password))
      return request->requestAuthentication();
    request->send_P(200, "text/plain", chart().c_str());
  });
  server.on("/phpmyadmin", HTTP_GET, [](AsyncWebServerRequest *request) {
    if (!request->authenticate(http_username, http_password))
      return request->requestAuthentication();
    request->send_P(200, "text/plain", phpMyAdmin().c_str());
  });
  //-------------------------------------------------------------------
  server.on("/M", HTTP_GET, [](AsyncWebServerRequest *request) {
    if (!request->authenticate(http_username, http_password))
      return request->requestAuthentication();
    lora_ctrl_mode = true;
    //loraSend(node1 + "M");
    request->send(200, "text/html", "sending command via lora to mushroom node...");                                   
  });

  server.on("/requestupdate", HTTP_GET, [](AsyncWebServerRequest *request) {
    if (!request->authenticate(http_username, http_password))
      return request->requestAuthentication();
    lora_request_update = true;
    //loraSend(node1 + "R");
    request->send(200, "text/html", "sending command via lora to mushroom node...");                                   
  });

  server.on("/P", HTTP_GET, [](AsyncWebServerRequest *request) {
    if (!request->authenticate(http_username, http_password))
      return request->requestAuthentication();
    lora_ctrl_pump = true;
    //loraSend(node1 + "P");
    request->send(200, "text/html", "sending command via lora to mushroom node...");
  });

  server.on("/setting", HTTP_GET, [](AsyncWebServerRequest *request) {
    if (!request->authenticate(http_username, http_password))
      return request->requestAuthentication();
    request->send(200, "text/html", setting_html);
  });

  server.on("/F", HTTP_GET, [](AsyncWebServerRequest *request) {
    if (!request->authenticate(http_username, http_password))
      return request->requestAuthentication();
    lora_ctrl_fan = true;
    //loraSend(node1 + "F");
    request->send(200, "text/html", "sending command via lora to mushroom node...");
  });

  // Send a GET request to <ESP_IP>/get?input1=<inputMessage>
  server.on("/get", HTTP_GET, [](AsyncWebServerRequest *request) {
    if (!request->authenticate(http_username, http_password))
      return request->requestAuthentication();
    // GET input1 value on <ESP_IP>/get?input1=<inputMessage>
    if (request->hasParam(PARAM_INPUT_1))
    {
      inputMessage = request->getParam(PARAM_INPUT_1)->value();
      inputParam = PARAM_INPUT_1;
      //loraSend(node1 + "T" + inputMessage);
    }

    // GET input2 value on <ESP_IP>/get?input2=<inputMessage>
    else if (request->hasParam(PARAM_INPUT_2))
    {
      inputMessage = request->getParam(PARAM_INPUT_2)->value();
      inputParam = PARAM_INPUT_2;
      //loraSend(node1 + "H" + inputMessage);
    }
    // GET input3 value on <ESP_IP>/get?input3=<inputMessage>
    else if (request->hasParam(PARAM_INPUT_3))
    {
      inputMessage = request->getParam(PARAM_INPUT_3)->value();
      inputParam = PARAM_INPUT_3;
      //loraSend(node1 + "Y" + inputMessage);
    }
    // GET input4 value on <ESP_IP>/get?input4=<inputMessage>
    else if (request->hasParam(PARAM_INPUT_4))
    {
      inputMessage = request->getParam(PARAM_INPUT_4)->value();
      inputParam = PARAM_INPUT_4;
      //loraSend(node1 + "J" + inputMessage);
    }

    else
    {
      inputMessage = "No message sent";
      inputParam = "none";
    }
    //Serial.println(inputMessage);
    request->send(200, "text/html", "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">"
                                    "<style>body{text-align:center;}</style>"
                                    "HTTP GET request sent to your ESP on input field (" +
                                        inputParam + ") with value: " + inputMessage + "<br><br><a href=\"/\">Return to Home Page</a><br>"
                                                                                       "<br><a href=\"/setting\">Return to setting page</a>");
  });
  
  server.on("/logout", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(401);
  });

  server.on("/logged-out", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send_P(200, "text/html", logout_html, processor);
  });

  // Start server
  server.begin();

  // Init and get the time
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);

  loraSend(node1 + "R");
}

void loop(void)
{

  unsigned long currentMillis = millis();
  
  while (Serial.available())
  { //Serial from VB
    line = Serial.readString();
    loraSend(node1 + line);
  }
  line = "";

  // try to parse packet
  if (LoRa.parsePacket())
  {
    // received a packet
    //Serial.print("Received packet '");
    // read packet
    while (LoRa.available())
    {
      if (LoRa.find(ipAddr))
      { // ip address this device "
        x = LoRa.readString();
        dataForVB = x.substring(2, 28);
        Serial.println(dataForVB);
      }
    }
  }
  node = x.substring(0, 2);
  if (node == "X1")
  {
    temp = x.substring(3, 8).toFloat();
    humi = x.substring(9, 14).toFloat();
    set_temp_max = x.substring(14, 16).toInt();
    set_humi_min = x.substring(16, 18).toInt();
    ctrlMode = x.substring(19, 20).toInt();
    pumpState = x.substring(21, 22).toInt();
    fanState = x.substring(23, 24).toInt();
    set_temp_min = x.substring(24, 26).toInt();
    set_humi_max = x.substring(26, 28).toInt();
  }
  //for request data via lora every 60s
  if (currentMillis - previousMillis_request >= interval_request)
  {
    previousMillis_request = currentMillis;
    loraSend(node1 + "R");
  }

  //for print data to VB(Windows App) every 1s
  if (currentMillis - previousMillis_vb >= interval_vb)
  {
    previousMillis_vb = currentMillis;
    Serial.println(dataForVB);
  }

  /*//for insert data into database server
  if (currentMillis - previousMillis_db >= interval_db) {
    previousMillis_db = currentMillis;
      insertDB();
  }  */
  //------------------------------------------------------------------------
  //Handle local time from NTP (Network Time Protocal)
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo))
  {
//    Serial.println("Failed to obtain time");
//    return;
  }
  char timeSec[3];
  strftime(timeSec, 3, "%S", &timeinfo); //Second
  char timeMin[3];
  strftime(timeMin, 3, "%M", &timeinfo); //Minute

  //Insert data into database every 1hr etc.(12:00:00) HH:MM:SS
  const char *second_db = "00";
  const char *minute_db = "00";
  if (strcmp(timeSec, second_db) == 0 && strcmp(timeMin, minute_db) == 0)
  {
    insertDB();
    delay(1000);
    //Serial.println("Insert into DB...");
  }
  //--------------------------------------------------------------------

  if (lora_ctrl_mode == true)
  {
    loraSend(node1 + "M");
    lora_ctrl_mode = false;
  }
  if (lora_ctrl_pump == true)
  {
    loraSend(node1 + "P");
    lora_ctrl_pump = false;
  }
  if (lora_ctrl_fan == true)
  {
    loraSend(node1 + "F");
    lora_ctrl_fan = false;
  }
  if (lora_request_update == true)
  {
    loraSend(node1 + "R");
    lora_request_update = false;
  }
  if (inputParam == "input1")
  {
    loraSend(node1 + "T" + inputMessage);
  }
  if (inputParam == "input2")
  {
    loraSend(node1 + "H" + inputMessage);
  }
  if (inputParam == "input3")
  {
    loraSend(node1 + "Y" + inputMessage);
  }
  if (inputParam == "input4")
  {
    loraSend(node1 + "J" + inputMessage);
  }
  inputParam = "";
}
