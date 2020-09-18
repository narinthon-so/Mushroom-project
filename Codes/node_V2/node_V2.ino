//VB + LoRa

#include "heltec.h"
#include <EEPROM.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

#define EEPROM_SIZE 512
#define BAND    433E6  //you can set band here directly,e.g. 868E6,915E6
#define sw1 12 //sw for change mode
#define sw2 13 //sw for control pump
#define sw3 15 //sw for control fan
#define pump 33
#define fan 4

#define R_sensing_pump 25
#define R_sensing_fan 35
int adc_pump_value, adc_fan_value = 0;
float Rs_pump_voltage, Rs_fan_voltage = 0;
bool pump_check, fan_check;
int last_pump_check = pump_check;
int last_fan_check = fan_check;

bool ctrlMode = false;
bool pumpState = false;
bool fanState = false;

const char* ipAddr = "X1";
const char* des = "X0";
String x;

String line = "";
int dataVB = 0;

//#include "DHT.h"
//#define DHTPIN 16
//#define DHTTYPE DHT22
//DHT dht(DHTPIN, DHTTYPE);

#include <Adafruit_AM2315.h>
Adafruit_AM2315 am2315;
// Connect RED of the AM2315 sensor to 5.0V
// Connect BLACK to Ground
// Connect WHITE to i2c clock
// Connect YELLOW to i2c data

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
  digitalWrite(pump, HIGH);
  digitalWrite(fan, HIGH);

  EEPROM.begin(EEPROM_SIZE);
  Serial.begin(115200);
  Wire.begin (21, 22);   // sda= GPIO_21 /scl= GPIO_22

  // Wake up the sensor
  Wire.beginTransmission(AM2315_I2CADDR);
  delay(2);
  Wire.endTransmission();

  while (!Serial) {
    delay(10);
  }

  analogReadResolution(12);
  //dht.begin();

  lcd.begin();                      // initialize the lcd
  //lcd.backlight();

  set_temp_min = EEPROM.read(1);
  set_temp_max = EEPROM.read(0);
  set_humi_min = EEPROM.read(10);
  set_humi_max = EEPROM.read(9);

  int count = 0;
  while (! am2315.begin()) {
    Serial.println("Sensor not found, check wiring & pullups!");
    delay(1000);
    count++;
    if (count == 30) {
      count = 0;
      break;
    }
  }
}

void loop() {

  last_pump_check = pump_check;
  last_fan_check = fan_check;

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
  //--------------------------------------------R sensing
  // Reading adc values
  adc_pump_value = analogRead(R_sensing_pump);
  adc_fan_value = analogRead(R_sensing_fan);
  //Serial.print(adc_pump_value); Serial.print("  "); Serial.println(adc_fan_value);

  //calculate to voltage
  Rs_pump_voltage = (adc_pump_value * 3.3 / 4095);
  Rs_fan_voltage = (adc_fan_value * 3.3 / 4095);
  //Serial.print(Rs_pump_voltage); Serial.print("  "); Serial.println(Rs_fan_voltage);
  if (Rs_pump_voltage > 0.01) {
    pump_check = true;
  } else {
    pump_check = false;
  }
  if (Rs_fan_voltage > 0.10) {
    fan_check = true;
  } else {
    fan_check = false;
  }

  if (pump_check != last_pump_check) {
    sendUpdateData();
  }
  last_pump_check = pump_check;

  if (fan_check != last_fan_check) {
    sendUpdateData();
  }
  last_fan_check = fan_check;
  //------------------------------------------------------------

  //  temp = dht.readTemperature();
  //  humi = dht.readHumidity();
  am2315.readTemperatureAndHumidity(&temp, &humi);

  //serial ----------------------------------------------------------------------
  while (Serial.available()) { //Serial from VB
    char  i = Serial.read();
    line += i;
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Serial Received");
    lcd.setCursor(0, 1);
    lcd.print("Keyword = " + line);
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
    if (pumpState == false) {
      pumpState = true;
    } else {
      pumpState = false;
    }
    sendUpdateData();
  } else if (Head == "F") {
    if (fanState == false) {
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
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Lora Received...");
    while (LoRa.available()) {
      if (LoRa.find(ipAddr)) {
        x = LoRa.readString();
        //Serial.println(x);
        lcd.setCursor(0, 1);
        lcd.print("Keyword = " + x);
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
    //temperature and humidity control
    if (temp < set_temp_min && humi < set_humi_min) {
      pumpState = true; fanState = false;
    }
    if (temp < set_temp_min && (humi >= set_humi_min && humi <= set_humi_max)) {
      pumpState = false; fanState = false;
    }
    if (temp < set_temp_min && humi > set_humi_max) {
      pumpState = false; fanState = true;
    }
    if ((temp >= set_temp_min && temp <= set_temp_max) && humi < set_humi_min) {
      pumpState = true; fanState = false;
    }
    if ((temp >= set_temp_min && temp <= set_temp_max) && (humi >= set_humi_min && humi <= set_humi_max)) {
      pumpState = false; fanState = false;
    }
    if ((temp >= set_temp_min && temp <= set_temp_max) && humi > set_humi_max) {
      pumpState = false; fanState = true;
    }
    if (temp > set_temp_max && humi < set_humi_min) {
      pumpState = true; fanState = true;
    }
    if (temp > set_temp_max && (humi >= set_humi_min && humi <= set_humi_max)) {
      pumpState = false; fanState = true;
    }
    if (temp > set_temp_max && humi > set_humi_max) {
      pumpState = false; fanState = true;
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
    lcd.clear();
    Serial.print("T");
    lcd.setCursor(0, 0);
    lcd.print("T:");
    if (temp < 10.00) {
      String tempStr = "0" + String(temp);
      Serial.print(tempStr);
      lcd.print(tempStr);
    } else {
      Serial.print(temp);
      lcd.print(temp);
    }
    Serial.print("H");
    lcd.setCursor(0, 1);
    lcd.print("H:");
    if (humi < 10.00) {
      String humiStr = "0" + String(humi);
      Serial.print(humiStr);
      lcd.print(humiStr);
    } else {
      Serial.print(humi);
      lcd.print(humi);
    }
    Serial.print(set_temp_max);
    Serial.print(set_humi_min);
    Serial.print("M");
    lcd.setCursor(12, 0);
    lcd.print("MODE");
    Serial.print(ctrlMode);
    lcd.setCursor(12, 1);
    if (ctrlMode) {
      lcd.print("MAN ");
    } else {
      lcd.print("AUTO");
    }
    Serial.print("P");
    lcd.setCursor(8, 0);
    lcd.print("P:");
    Serial.print(pump_check);
    lcd.print(pump_check);
    Serial.print("F");
    lcd.setCursor(8, 1);
    lcd.print("F:");
    Serial.print(fan_check);
    lcd.print(fan_check);
    Serial.print(set_temp_min);
    Serial.print(set_humi_max);
    Serial.println();
  }

  //digitalWrite(pump, pumpState);
  //digitalWrite(fan, fanState);

  //-------------------------------Active low relays
  if (pumpState == true) {
    digitalWrite(pump, false);
  } else {
    digitalWrite(pump, true);
  }
  if (fanState == true) {
    digitalWrite(fan, false);
  } else {
    digitalWrite(fan, true);
  }
  //-----------------------------------------
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
  //--------------------------------------------R sensing
  // Reading adc values
  adc_pump_value = analogRead(R_sensing_pump);
  adc_fan_value = analogRead(R_sensing_fan);
  //Serial.print(adc_pump_value); Serial.print("  "); Serial.println(adc_fan_value);

  //calculate to voltage
  Rs_pump_voltage = (adc_pump_value * 3.3 / 4095);
  Rs_fan_voltage = (adc_fan_value * 3.3 / 4095);
  //Serial.print(Rs_pump_voltage); Serial.print("  "); Serial.println(Rs_fan_voltage);
  if (Rs_pump_voltage > 0.01) {
    pump_check = true;
  } else {
    pump_check = false;
  }
  if (Rs_fan_voltage > 0.10) {
    fan_check = true;
  } else {
    fan_check = false;
  }
  //------------------------------------------------------------
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
  LoRa.print(pump_check);
  LoRa.print("F");
  LoRa.print(fan_check);
  LoRa.print(set_temp_min);
  LoRa.print(set_humi_max);
  LoRa.endPacket();
}
