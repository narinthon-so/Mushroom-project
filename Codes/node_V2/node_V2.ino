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

float temp, humi;

int set_temp_min, set_temp_max; /*T => set_temp_max*/ /*Y => set_temp_min*/
int set_humi_min, set_humi_max; /*H => set_humi_min*/ /*J => set_humi_max*/

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

  set_temp_min = EEPROM.read(1);
  set_temp_max = EEPROM.read(0);
  set_humi_min = EEPROM.read(10);
  set_humi_max = EEPROM.read(9);

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

  //serial ----------------------------------------------------------------------
  while (Serial.available()) { //Serial from VB
    char  i = Serial.read();
    line += i;
  }
  String Head = line.substring(0, 1);
  dataVB = line.substring(1, 3).toInt();  // real da sub 3 time EX H20

  if (Head == "Y") {
    set_temp_min = dataVB;
    EEPROM.write(1, set_temp_min);
    EEPROM.commit();
    sendUpdateData();
    line = "";

  } else if (Head == "T") {
    set_temp_max = dataVB;
    EEPROM.write(0, set_temp_max);
    EEPROM.commit();
    sendUpdateData();
    line = "";

  } else if (Head == "H") {
    set_humi_min = dataVB;
    EEPROM.write(10, set_humi_min);
    EEPROM.commit();
    sendUpdateData();
    line = "";
  } else if (Head == "J") {
    set_humi_max = dataVB;
    EEPROM.write(9, set_humi_max);
    EEPROM.commit();
    sendUpdateData();
    line = "";
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



  //LoRa--------------------------------------------------------------
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
        else if (head == "Y") {
          set_temp_min = dataLR;
          EEPROM.write(1, set_temp_min);
          EEPROM.commit();
          sendUpdateData();
        }
        else if (head == "T") {
          set_temp_max = dataLR;
          EEPROM.write(0, set_temp_max);
          EEPROM.commit();
          sendUpdateData();
        }
        else if (head == "H") {
          set_humi_min = dataLR;
          EEPROM.write(10, set_humi_min);
          EEPROM.commit();
          sendUpdateData();
        }
        else if (head == "J") {
          set_humi_max = dataLR;
          EEPROM.write(9, set_humi_max);
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

  //check mode-------------------------------------------------------------
  if (ctrlMode == false) { // auto mode

    if (humi < set_humi_min) {
      pumpState = true;
    } else {
      pumpState = false;
    }

    if (humi > set_humi_max) {
      pumpState = false;
    } else {
      pumpState = true;
    }

    if (temp > set_temp_max) {
      fanState = true; //ลดอุณหภูมิ
    } else {
      fanState = false;
    }

    if (temp < set_temp_min) {
      //เพิ่มอุณหภูมิ
    } else {
      //noting
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

  //print data to VB-------------------------------------------------
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
    Serial.print(set_temp_max);
    Serial.print(set_humi_min);
    Serial.print("M");
    Serial.print(ctrlMode);
    Serial.print("P");
    Serial.print(pumpState);
    Serial.print("F");
    Serial.print(fanState);
    Serial.print(set_temp_min);
    Serial.print(set_humi_max);
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

    if (humi < set_humi_min) {
      pumpState = true;
    } else {
      pumpState = false;
    }

    if (humi > set_humi_max) {
      pumpState = false;
    } else {
      pumpState = true;
    }

    if (temp > set_temp_max) {
      fanState = true; //ลดอุณหภูมิ
    } else {
      fanState = false;
    }

    if (temp < set_temp_min) {
      //เพิ่มอุณหภูมิ
    } else {
      //noting
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
  LoRa.print(set_temp_max);
  LoRa.print(set_humi_min);
  LoRa.print("M");
  LoRa.print(ctrlMode);
  LoRa.print("P");
  LoRa.print(pumpState);
  LoRa.print("F");
  LoRa.print(fanState);
  LoRa.print(set_temp_min);
  LoRa.print(set_humi_max);
  LoRa.endPacket();
}
