/*
  Rui Santos
  Complete project details at Complete project details at https://RandomNerdTutorials.com/esp32-http-get-post-arduino/

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files.

  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.
*/

#include <WiFi.h>
#include <HTTPClient.h>
#include <Arduino_JSON.h>

HTTPClient http;

const char* ssid = "ASUS";
const char* password = "123456789";

//Your Domain name with URL path or IP address with path
const char* serverName = "http://192.168.43.181/public-url.php";

String json;
String tunnelsNameArr[2];
String publicURLArr[2];
String database_server_url, web_server_url;

void setup() {
  Serial.begin(115200);

  WiFi.begin(ssid, password);
  Serial.println("Connecting");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());

  //Serial.println("Timer set to 5 seconds (timerDelay variable), it will take 5 seconds before publishing the first reading.");
  //Check WiFi connection status
  if (WiFi.status() == WL_CONNECTED) {

    json = httpGETRequest(serverName);
    Serial.println(json);
    JSONVar myObject = JSON.parse(json);

    // JSON.typeof(jsonVar) can be used to get the type of the var
    if (JSON.typeof(myObject) == "undefined") {
      Serial.println("Parsing input failed!");
      return;
    }

    Serial.print("JSON object = ");
    Serial.println(myObject);

    // myObject.keys() can be used to get an array of all the keys in the object
    JSONVar keys = myObject.keys();

    for (int i = 0; i < keys.length(); i++) {
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
  else {
    Serial.println("WiFi Disconnected");
  }

  if(tunnelsNameArr[0] == "web_server"){
    web_server_url = publicURLArr[0];
    database_server_url = publicURLArr[1];
  }else{
    web_server_url = publicURLArr[1];
    database_server_url = publicURLArr[0];
  }

  Serial.println(web_server_url);
  Serial.println(database_server_url);
}

void loop() {

}

String httpGETRequest(const char* serverName) {

  // Your IP address with path or Domain name with URL path
  http.begin(serverName);

  // Send HTTP POST request
  int httpResponseCode = http.GET();

  String payload = "{}";

  if (httpResponseCode > 0) {
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
    payload = http.getString();
  }
  else {
    Serial.print("Error code: ");
    Serial.println(httpResponseCode);
  }
  // Free resources
  http.end();

  return payload;
}
