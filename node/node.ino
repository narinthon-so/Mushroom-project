//VB + LoRa

#include "heltec.h"
#include <EEPROM.h>

#define EEPROM_SIZE 512
#define BAND    433E6  //you can set band here directly,e.g. 868E6,915E6
#define sw1 12 //sw for change mode
#define sw2 13 //sw for control pump
#define sw3 15 //sw for control fan
#define pump 33
#define fan 4

bool ctrlMode = false;
bool pumpState = false;
bool fanState = false;

const char* ipAddr = "X1";
const char* des = "X0";
String x;

String line = "";
int dataVB = 0;

#include "DHT.h"
#define DHTPIN 16
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

unsigned long previousMillis = 0;
const long interval = 1000;

float temp;
float humi;

int setTemp;
int setHumi;


void setup() {

  Heltec.begin(false /*DisplayEnable Enable*/, true /*Heltec.LoRa Disable*/, true /*Serial Enable*/, true /*PABOOST Enable*/, BAND /*long BAND*/);
  pinMode(sw1, INPUT_PULLUP);
  pinMode(sw2, INPUT_PULLUP);
  pinMode(sw3, INPUT_PULLUP);

  pinMode(pump, OUTPUT);
  pinMode(fan, OUTPUT);

  EEPROM.begin(EEPROM_SIZE);
  Serial.begin(115200);
  dht.begin();
  setTemp = EEPROM.read(0);
  setHumi = EEPROM.read(10);
}

void loop() {

  //change mode
  if (digitalRead(sw1) == 0) {
    while (digitalRead(sw1) == 0);
    delay(200);
    if (ctrlMode == false) {
      ctrlMode = true;
    }
    else {
      ctrlMode = false;
    }
    sendUpdateData();
  }

  temp = dht.readTemperature();
  humi = dht.readHumidity();

  while (Serial.available()) { //Serial from VB
    char  i = Serial.read();
    line += i;
  }
  String Head = line.substring(0, 1);
  dataVB = line.substring(1, 3).toInt();  // real da sub 3 time EX H20
  // sub 4 time for EX H100

  if (Head == "T") {
    setTemp = dataVB;
    EEPROM.write(0, setTemp);
    EEPROM.commit();
    sendUpdateData();
    line = "";

  } else if (Head == "H") {
    setHumi = dataVB;
    EEPROM.write(10, setHumi);
    EEPROM.commit();
    sendUpdateData();
    line = "";
  } else if (Head == "R") {

    /*   Serial.print("T");
       Serial.print(EEPROM.read(0));
       Serial.print("H");
       Serial.print(EEPROM.read(10));
    */

  } else if (Head == "M") {
    if (ctrlMode == false) {
      ctrlMode = true;
    }
    else {
      ctrlMode = false;
    }
    sendUpdateData();
    line = "";
  } else if (Head == "P") {
    int state = line.substring(1, 2).toInt();
    if (state == 1) {
      pumpState = true;
    } else {
      pumpState = false;
    }
    sendUpdateData();
  } else if (Head == "F") {
    int state = line.substring(1, 2).toInt();
    if (state == 1) {
      fanState = true;
    } else {
      fanState = false;
    }

    sendUpdateData();
  }
  //reset line value
  line = "";



  //LoRa
  if (LoRa.parsePacket()) {
    // received a packet
    // Serial.print("Received packet '");
    // read packet
    while (LoRa.available()) {
      if (LoRa.find(ipAddr)) {
        x = LoRa.readString();
        //Serial.println(x);
        String head = x.substring(0, 1);
        int dataLR = x.substring(1, 3).toInt();
        if (head == "M") {
          if (ctrlMode == false) {
            ctrlMode = true;
          }
          else {
            ctrlMode = false;
          }
          sendUpdateData();
        }
        else if (head == "T") {
          setTemp = dataLR;
          EEPROM.write(0, setTemp);
          EEPROM.commit();
          sendUpdateData();
        }
        else if (head == "H") {
          setHumi = dataLR;
          EEPROM.write(10, setHumi);
          EEPROM.commit();
          sendUpdateData();
        }
        else if (head == "P") {
          if (pumpState == false) {
            pumpState = true;
          } else {
            pumpState = false;
          }
          sendUpdateData();
        }
        else if (head == "F") {
          if (fanState == false) {
            fanState = true;
          } else {
            fanState = false;
          }
          sendUpdateData();
        }
        else if (head == "R") {

          sendUpdateData();
        }

      }
    }
    // print RSSI of packet
    // Serial.print("' with RSSI ");
    // Serial.println(LoRa.packetRssi());
  }

  //check mode
  if (ctrlMode == false) { // auto mode

    if (humi < setHumi) {
      pumpState = true;
    } else {
      pumpState = false;
    }

    if (temp > setTemp) {
      fanState = true;
    } else {
      fanState = false;
    }

  }
  else if (ctrlMode == true) { // manual mode

    if (digitalRead(sw2) == 0) {  //pump
      while (digitalRead(sw2) == 0);
      delay(200);
      if (pumpState == false) {
        pumpState = true;
        sendUpdateData();
      }
      else {
        pumpState = false;
        sendUpdateData();
      }

    }  //pump

    if (digitalRead(sw3) == 0) {   //fan
      while (digitalRead(sw3) == 0);
      delay(200);
      if (fanState == false) {
        fanState = true;
        sendUpdateData();
      }
      else {
        fanState = false;
        sendUpdateData();
      }
    }  //fan



  }

  //print values to VB
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    Serial.print("T");
    if (temp < 10.00) {
      String tempStr = "0" + String(temp);
      Serial.print(tempStr);
    } else {
      Serial.print(temp);
    }
    Serial.print("H");
    if (humi < 10.00) {
      String humiStr = "0" + String(humi);
      Serial.print(humiStr);
    } else {
      Serial.print(humi);
    }
    Serial.print(setTemp);
    Serial.print(setHumi);
    Serial.print("M");
    Serial.print(ctrlMode);
    Serial.print("P");
    Serial.print(pumpState);
    Serial.print("F");
    Serial.print(fanState);
    Serial.println();
  }

  digitalWrite(pump, pumpState);
  digitalWrite(fan, fanState);
}

void loraSend(String datasend) {
  String ip = "X1";
  LoRa.beginPacket();
  LoRa.print(des + ip + datasend);
  LoRa.endPacket();
}

void sendUpdateData() { //this function will call when sumting change ...
  //--------------------------------------------------
  //change pumpState and fanState before sendUpdateData ***if not pumpState and fanState on webserver will not be real value
  if (ctrlMode == false) { // auto mode

    if (humi < setHumi) {
      pumpState = true;
    } else {
      pumpState = false;
    }

    if (temp > setTemp) {
      fanState = true;
    } else {
      fanState = false;
    }

  }
  //----------------------------------------------------
  LoRa.beginPacket();
  LoRa.print(String(des) + String(ipAddr));
  LoRa.print("T");
  if (temp < 10.00) {
    String tempStr = "0" + String(temp);
    LoRa.print(tempStr);
  } else {
    LoRa.print(temp);
  }
  LoRa.print("H");
  if (humi < 10.00) {
    String humiStr = "0" + String(humi);
    LoRa.print(humiStr);
  } else {
    LoRa.print(humi);
  }
  LoRa.print(setTemp);
  LoRa.print(setHumi);
  LoRa.print("M");
  LoRa.print(ctrlMode);
  LoRa.print("P");
  LoRa.print(pumpState);
  LoRa.print("F");
  LoRa.print(fanState);

  LoRa.endPacket();
}
