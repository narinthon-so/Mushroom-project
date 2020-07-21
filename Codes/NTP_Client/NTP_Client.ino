/*
  Rui Santos
  Complete project details at https://RandomNerdTutorials.com/esp32-date-time-ntp-client-server-arduino/

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files.

  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.
*/

#include <WiFi.h>
#include "time.h"
#include <string.h>
const char* ssid     = "ASUS";
const char* password = "123456789";

const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 21600;     //Thailand time zone
const int   daylightOffset_sec = 3600;

void setup() {
  Serial.begin(115200);

  // Connect to Wi-Fi
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected.");

  // Init and get the time
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);

}

void loop() {

  delay(1000);
  //Handle local time from NTP Network Time Protocal
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    Serial.println("Failed to obtain time");
    return;
  }
  char timeSec[3];
  strftime(timeSec, 3, "%S", &timeinfo); //Second
  char timeMin[3];
  strftime(timeMin, 3, "%M", &timeinfo); //Minute
  //--------------------------------------------------------------------
  const char* second_30 = "30"; //Request values via lora every 30s
  const char* second_00 = "00";
  if (strcmp(timeSec, second_30)  == 0 || strcmp(timeSec, second_00) == 0) {
    Serial.println("Lora request...");
  }
  //--------------------------------------------------------------------
  //Insert data into database every 1hr etc.(12:00:00) HH:MM:SS
  const char* second_db = "00";
  const char* minute_db = "00";
  if (strcmp(timeSec, second_db)  == 0 && strcmp(timeMin, minute_db) == 0) {
    Serial.println("Insert into DB...");
  }
  //--------------------------------------------------------------------
}

void printLocalTime() {
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    Serial.println("Failed to obtain time");
    return;
  }
  Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");
  Serial.print("Day of week: ");
  Serial.println(&timeinfo, "%A");
  Serial.print("Month: ");
  Serial.println(&timeinfo, "%B");
  Serial.print("Day of Month: ");
  Serial.println(&timeinfo, "%d");
  Serial.print("Year: ");
  Serial.println(&timeinfo, "%Y");
  Serial.print("Hour: ");
  Serial.println(&timeinfo, "%H");
  Serial.print("Hour (12 hour format): ");
  Serial.println(&timeinfo, "%I");
  Serial.print("Minute: ");
  Serial.println(&timeinfo, "%M");
  Serial.print("Second: ");
  Serial.println(&timeinfo, "%S");

  Serial.println("Time variables");
  char timeHour[3];
  strftime(timeHour, 3, "%H", &timeinfo);
  Serial.println(timeHour);
  char timeWeekDay[10];
  strftime(timeWeekDay, 10, "%A", &timeinfo);
  Serial.println(timeWeekDay);
  char timeSec[3];
  strftime(timeSec, 3, "%S", &timeinfo);
  Serial.println(timeSec);
  Serial.println();
}
