//14532061

// Load Wi-Fi library
#include <WiFi.h>
#include "heltec.h"
#define BAND    433E6
#define led1 2

String node, x;
float temp ;
float humi;
int setTemp;
int setHumi;
bool ctrlMode, pumpState, fanState;

// Replace with your network credentials
const char* ssid     = "ASUS";
const char* password = "123456789";

const char* ipAddr = "X0";
const char* node1 = "X1";

// Current time
unsigned long currentTime = millis();
// Previous time
unsigned long previousTime = 0;
// Define timeout time in milliseconds (example: 2000ms = 2s)
const long timeoutTime = 2000;

// Set web server port number to 80
WiFiServer server(80);

// Variable to store the HTTP request
String header;

// Auxiliar variables to store the current output state
String led1State = "off";

void setup() {
  Serial.begin(115200);
  Heltec.begin(0/*DisplayEnable Enable*/, true /*Heltec.LoRa Disable*/, true /*Serial Enable*/, true /*PABOOST Enable*/, BAND /*long BAND*/);

  Serial.print("connecting to...");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");

  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  // Initialize the output variables as outputs
  pinMode(led1, OUTPUT);
  // Set outputs to LOW
  digitalWrite(led1, LOW);

  server.begin();

  loraSend(String(node1) + "R"); // request values

}

void loop() {

  WiFiClient client = server.available();   // Listen for incoming clients
  
  if (LoRa.parsePacket()) {
    // received a packet
    //Serial.print("Received packet '");
    // read packet
    while (LoRa.available()) {
      if (LoRa.find(ipAddr)) { // ip address this device
        x = LoRa.readString();
        Serial.println(x);            //ส่ง Data ให้ VB
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
      }
    }
  }
  if (client) {                             // If a new client connects,
    Serial.println("New Client.");          // print a message out in the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    currentTime = millis();
    previousTime = currentTime;
    while (client.connected() && currentTime - previousTime <= timeoutTime) { // loop while the client's connected
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor
        header += c;
        if (c == '\n') {                    // if the byte is a newline character
          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();

            // turns the GPIOs on and off
            if (header.indexOf("GET /2/on") >= 0) {
              Serial.println("GPIO 2 on");
              led1State = "on";
              digitalWrite(led1, HIGH);
            } else if (header.indexOf("GET /2/off") >= 0) {
              Serial.println("GPIO 2 off");
              led1State = "off";
              digitalWrite(led1, LOW);
            } else if (header.indexOf("GET /P") >= 0) {
              loraSend(String(node1) + "P");
              Serial.println("changing pump state...");
            } else if (header.indexOf("GET /F") >= 0) {
              loraSend(String(node1) + "F");
              Serial.println("changing fan state...");
            } else if (header.indexOf("GET /M") >= 0) {
              loraSend(String(node1) + "M");
              Serial.println("changing mode...");
            }

            // Display the HTML web page
            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");
            // CSS to style the on/off buttons
            // Feel free to change the background-color and font-size attributes to fit your preferences
            client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
            client.println(".button { background-color: #4CAF50; border: none; color: white; padding: 16px 40px;");
            client.println("text-decoration: none; font-size: 20px; margin: 2px; cursor: pointer;}");
            client.println(".button2 {background-color: #555555;}.button3 {background-color: #008CBA;}</style></head>");

            // Web Page Heading
            client.println("<body><h1>WiFi LoRa 32 Web Server</h1>");

            // Display current state, and ON/OFF buttons for GPIO 2
            client.println("<p>led1 - State " + led1State + "</p>");
            // If the output26State is off, it displays the ON button
            if (led1State == "off") {
              client.println("<p><a href=\"/2/on\"><button class=\"button\">ON</button></a></p>");
            } else {
              client.println("<p><a href=\"/2/off\"><button class=\"button button2\">OFF</button></a></p>");
            }

            client.println("<p>Temperature: " + String(temp) + " *C</p>");
            client.println("<p>Humidity: " + String(humi) + " %</p>");
            client.println("<p>Pumpstate: " + String(pumpState) + "</p>");
            client.println("<p>Fanstate: " + String(fanState) + " </p>");
            client.println("<p>Mode: " + String(ctrlMode) + " </p>");


            if (pumpState == 0) {
              client.println("<p><a href=\"/P\"><button class=\"button\">Pump ON</button></a></p>");
            } else {
              client.println("<p><a href=\"/P\"><button class=\"button button2\">Pump OFF</button></a></p>");
            }
            if (fanState == 0) {
              client.println("<p><a href=\"/F\"><button class=\"button\">Fan ON</button></a></p>");
            } else {
              client.println("<p><a href=\"/F\"><button class=\"button button2\">Fan OFF</button></a></p>");
            }

            client.println("<p><a href=\"/M\"><button class=\"button button3\">Changemode</button></a></p>");


            client.println("</body></html>");
            // The HTTP response ends with another blank line
            client.println();
            // Break out of the while loop
            break;
          } else { // if you got a newline, then clear currentLine
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }
      }

    }
    // Clear the header variable
    header = "";
    // Close the connection
    client.stop();
    Serial.println("Client disconnected.");
    Serial.println("");
  }
  // try to parse packet

}



void loraSend(String datasend) {
  LoRa.beginPacket();
  LoRa.print(datasend);
  LoRa.endPacket();
}
