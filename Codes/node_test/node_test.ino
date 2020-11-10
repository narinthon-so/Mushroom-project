//VB + LoRa

#include "heltec.h"
#include <EEPROM.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <BH1750.h>
//#include "esp_system.h"

LiquidCrystal_I2C lcd(0x27, 16, 2);
BH1750 lightMeter;

#define EEPROM_SIZE 512
#define BAND    433E6  //you can set band here directly,e.g. 868E6,915E6
#define sw1 12 //sw for change mode
#define sw2 13 //sw for control pump
#define sw3 15 //sw for control fan
#define pump 33
#define fan 4
/*
  //******************************** LDR *****************************************
  // These constants, define values needed for the LDR readings and ADC
  #define LDR_PIN                   25
  #define MAX_ADC_READING           4095
  #define ADC_REF_VOLTAGE           3.3
  #define REF_RESISTANCE            5030  // measure this for best results
  #define LUX_CALC_SCALAR           12518931
  #define LUX_CALC_EXPONENT         -1.405
  int   ldrRawData;
  float resistorVoltage, ldrVoltage;
  float ldrResistance;
  //**************************************************************************/
float lux;
int set_lux; //0-999
const int ledPin = 2;
const int ledPin_2 = 0;
bool ledPin_2_sate;
// setting PWM properties
const int freq = 547;
const int ledChannel = 0;
const int resolution = 10;
int pwmvalue;
//**************************************************************************//

#define R_sensing_pump 34
#define R_sensing_fan 35
int adc_pump_value, adc_fan_value;
float Rs_pump_voltage, Rs_fan_voltage;
bool pump_check, fan_check;
bool last_pump_check = pump_check;
bool last_fan_check = fan_check;

bool ctrlMode;
bool pumpState = false;
bool fanState = false;
bool day;

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
unsigned long previousMillis_am2315 = 0;
const int interval = 1000;
const int interval_am2315 = 2000;


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
  pinMode(ledPin_2, OUTPUT);
  digitalWrite(pump, HIGH);
  digitalWrite(fan, HIGH);
  digitalWrite(ledPin_2, HIGH);

  // configure LED PWM functionalitites
  ledcSetup(ledChannel, freq, resolution);

  // attach the channel to the GPIO to be controlled
  ledcAttachPin(ledPin, ledChannel);

  EEPROM.begin(EEPROM_SIZE);
  Serial.begin(115200);

  Wire.begin(21, 22, 100000);   // sda= GPIO_21 /scl= GPIO_22
  //Wire1.begin(16, 17, 100000); //sda_2= GPIO_16 /scl_2= GPIO_17
  //lightMeter.begin(BH1750::ONE_TIME_HIGH_RES_MODE);

  // Wake up the sensor
  Wire.beginTransmission(AM2315_I2CADDR);
  delay(2);
  Wire.endTransmission();

  while (!Serial) {
    delay(10);
  }

  analogReadResolution(12);
  //dht.begin();

  // initialize LCD
  lcd.init();
  // turn on LCD backlight
  lcd.backlight();

  //  EEPROM.write(2, 2);
  //  EEPROM.write(3, 0);
  //  EEPROM.write(4, 0);
  //  EEPROM.commit();

  set_temp_min = EEPROM.read(1);
  set_temp_max = EEPROM.read(0);
  set_humi_min = EEPROM.read(10);
  set_humi_max = EEPROM.read(9);
  day = EEPROM.read(20);
  ctrlMode = EEPROM.read(7);

  int I, II, III;
  I = EEPROM.read(2);
  II = EEPROM.read(3);
  III = EEPROM.read(4);

  //  Serial.println(I);
  //  Serial.println(II);
  //  Serial.println(III);

  if (I == 0) {
    set_lux = (String(II) + String(III)).toInt();
  } else {
    set_lux = (String(I) + String(II) + String(III)).toInt();
  }
    
  lcd.clear();
  
  int count = 0;
  while (! am2315.begin()) {
    lcd.setCursor(0, 0);
    lcd.print("CONNECTING AM2315");
    Serial.println("Sensor not found, check wiring & pullups!");
    delay(1000);
    count++;
    if (count == 15) {
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
    EEPROM.write(7, ctrlMode);
    EEPROM.commit();
    sendUpdateData();
  }

  /*//********************************* LDR Lux Meter *******************************
    // Perform the analog to digital conversion
    ldrRawData = analogRead(LDR_PIN);

    // RESISTOR VOLTAGE_CONVERSION
    // Convert the raw digital data back to the voltage that was measured on the analog pin
    resistorVoltage = (float)ldrRawData / MAX_ADC_READING * ADC_REF_VOLTAGE;

    // voltage across the LDR is the 3.3V supply minus the 5k resistor voltage
    ldrVoltage = ADC_REF_VOLTAGE - resistorVoltage;

    // LDR_RESISTANCE_CONVERSION
    // resistance that the LDR would have for that voltage
    ldrResistance = ldrVoltage / resistorVoltage * REF_RESISTANCE;

    // LDR_LUX
    // Change the code below to the proper conversion from ldrResistance to
    // lux
    lux = LUX_CALC_SCALAR * pow(ldrResistance, LUX_CALC_EXPONENT);
    //**********************************************************************************/
  //lux = lightMeter.readLightLevel();
  lux = 203.65;

  if (day) {
    if (lux < set_lux) {
      if (pwmvalue < 100) {
        pwmvalue++;
      }
      if (pwmvalue == 100) {
        pwmvalue = 100;
      }
      ledPin_2_sate = false;
    }
    else if (lux > set_lux) {
      if (pwmvalue > 0) {
        pwmvalue--;
      }
      if (pwmvalue == 0) {
        pwmvalue = 0;
      }
      ledPin_2_sate = true;
    }
    ledcWrite(ledChannel, pwm(pwmvalue));
    digitalWrite(ledPin_2, ledPin_2_sate);
    //Serial.println(day);
  }

  // print out the results
  //    Serial.print("LDR Raw Data   : "); Serial.println(ldrRawData);
  //    Serial.print("LDR Voltage    : "); Serial.print(ldrVoltage); Serial.println(" volts");
  //    Serial.print("LDR Resistance : "); Serial.print(ldrResistance); Serial.println(" Ohms");
  //    Serial.print("LDR Illuminance: "); Serial.print(lux); Serial.println(" lux");
  //    Serial.println(pwmvalue);
  //*****************************************************************************

  //R sensing ************************************************************************
  // Reading adc values
  for (int i = 0; i < 10; i++) {
    adc_pump_value = adc_pump_value + analogRead(R_sensing_pump);
  }
  adc_pump_value = adc_pump_value / 10;

  adc_fan_value = analogRead(R_sensing_fan);
  //Serial.print(adc_pump_value); Serial.print("  "); Serial.println(adc_fan_value);

  //calculate to voltage
  Rs_pump_voltage = (adc_pump_value * 3.3 / 4095);
  Rs_fan_voltage = (adc_fan_value * 3.3 / 4095);
  //Serial.print(Rs_pump_voltage); Serial.print("  "); Serial.println(Rs_fan_voltage);
  if (Rs_pump_voltage >= 0.01) {
    pump_check = true;
  } else {
    pump_check = false;
  }
  if (Rs_fan_voltage >= 0.10) {
    fan_check = true;
  } else {
    fan_check = false;
  }

  if (pump_check != last_pump_check) {
    //sendUpdateData();
  }
  last_pump_check = pump_check;

  if (fan_check != last_fan_check) {
    //sendUpdateData();
  }
  last_fan_check = fan_check;
  //***************************************************************************

  //  temp = dht.readTemperature();
  //  humi = dht.readHumidity();
  //am2315.readTemperatureAndHumidity(&temp, &humi);
  //*****************************************************************************

  //serial **********************************************************************
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
    EEPROM.write(7, ctrlMode);
    EEPROM.commit();
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
  } else if (Head == "L") {
    int serial_lux = line.substring(1, 4).toInt();
    set_lux = serial_lux;
    if (set_lux < 100) {
      String set_luxstr = String(set_lux);
      int I, II, III;
      I = 0;
      II = set_luxstr.substring(0, 1).toInt();
      III = set_luxstr.substring(1, 2).toInt();
      EEPROM.write(2, I);
      EEPROM.write(3, II);
      EEPROM.write(4, III);
      EEPROM.commit();
    } else {
      String set_luxstr = String(set_lux);
      int I, II, III;
      I = set_luxstr.substring(0, 1).toInt();
      II = set_luxstr.substring(1, 2).toInt();
      III = set_luxstr.substring(2, 3).toInt();
      EEPROM.write(2, I);
      EEPROM.write(3, II);
      EEPROM.write(4, III);
      EEPROM.commit();
    }
    sendUpdateData();
  }
  //reset line value
  line = "";
  //*****************************************************************************

  //LoRa ************************************************************************
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
          EEPROM.write(7, ctrlMode);
          EEPROM.commit();
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
        else if (head == "D") {
          int dta = x.substring(1, 2).toInt();
          if (dta) {
            day = true;
          } else {
            day = false;
          }
          EEPROM.write(20, day);
          EEPROM.commit();
          sendUpdateData();
        }
        else if (head == "L") {
          int lora_lux = x.substring(1, 4).toInt();
          set_lux = lora_lux;
          if (set_lux < 100) {
            String set_luxstr = String(set_lux);
            int I, II, III;
            I = 0;
            II = set_luxstr.substring(0, 1).toInt();
            III = set_luxstr.substring(1, 2).toInt();
            EEPROM.write(2, I);
            EEPROM.write(3, II);
            EEPROM.write(4, III);
            EEPROM.commit();
          } else {
            String set_luxstr = String(set_lux);
            int I, II, III;
            I = set_luxstr.substring(0, 1).toInt();
            II = set_luxstr.substring(1, 2).toInt();
            III = set_luxstr.substring(2, 3).toInt();
            EEPROM.write(2, I);
            EEPROM.write(3, II);
            EEPROM.write(4, III);
            EEPROM.commit();
          }
          sendUpdateData();
        }
        else if (head == "Z") {
          ESP.restart();       // คำสั่งรีเซ็ต ESP
          //esp_restart_noos();
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
  //************************************************************************
  //check mode ****************************************************************
  if (ctrlMode == false) { // auto mode
    //temperature and humidity control
    if (temp < set_temp_min && humi < set_humi_min) {
      pumpState = true; fanState = false;
    }
    if (temp < set_temp_min && (humi >= set_humi_min && humi <= set_humi_max)) {
      pumpState = false; fanState = false;
    }
    if (temp < set_temp_min && humi > set_humi_max) {
      pumpState = false; //fanState = true;
      fanState = false;
    }
    if ((temp >= set_temp_min && temp <= set_temp_max) && humi < set_humi_min) {
      pumpState = true; fanState = false;
    }
    if ((temp >= set_temp_min && temp <= set_temp_max) && (humi >= set_humi_min && humi <= set_humi_max)) {
      pumpState = false; fanState = false;
    }
    if ((temp >= set_temp_min && temp <= set_temp_max) && humi > set_humi_max) {
      pumpState = false; //fanState = true;
      fanState = false;
    }
    if (temp > set_temp_max && humi < set_humi_min) {
      pumpState = true; fanState = true;
    }
    if (temp > set_temp_max && (humi >= set_humi_min && humi <= set_humi_max)) {
      pumpState = true; fanState = true;
    }
    if (temp > set_temp_max && humi > set_humi_max) {
      pumpState = true; fanState = true;
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
  if (currentMillis - previousMillis_am2315 >= interval_am2315) {
    previousMillis_am2315 = currentMillis;
    //am2315.readTemperatureAndHumidity(&temp, &humi);
    temp = 25.30; humi = 85.60;
  }
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    lcd.clear();
    Serial.print(F("T"));
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
    Serial.print(F("H"));
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
    Serial.print(F("P"));
    lcd.setCursor(8, 0);
    lcd.print("P:");
    Serial.print(pump_check);
    lcd.print(pump_check);
    Serial.print(F("F"));
    lcd.setCursor(8, 1);
    lcd.print("F:");
    Serial.print(fan_check);
    lcd.print(fan_check);
    Serial.print(set_temp_min);
    Serial.print(set_humi_max);
    Serial.print(F("L"));
    if (lux < 10.00) {
      Serial.print("000" + String(lux));
    }
    else if (lux < 100.00) {
      Serial.print("00" + String(lux));
    }
    else if (lux < 1000.00) {
      Serial.print("0" + String(lux));
    }
    else if (lux >= 10000.00) {
      Serial.print("Rovflux");
    } else {
      Serial.print(lux);
    }
    if (set_lux < 100) {
      Serial.print("0" + String(set_lux));
    } else {
      Serial.print(set_lux);
    }
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

  //delay(500);
}

void loraSend(String datasend) {
  String ip = "X1";
  LoRa.beginPacket();
  LoRa.print(des + ip + datasend);
  LoRa.endPacket();
}

void sendUpdateData() { //this function will call when sumting change ...
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
  //LoRa.print(pump_check);
  LoRa.print(pumpState);
  LoRa.print("F");
  LoRa.print(fan_check);
  LoRa.print(set_temp_min);
  LoRa.print(set_humi_max);
  LoRa.print("L");
  if (lux < 10.00) {
    LoRa.print("000" + String(lux));
  }
  else if (lux < 100.00) {
    LoRa.print("00" + String(lux));
  }
  else if (lux < 1000.00) {
    LoRa.print("0" + String(lux));
  }
  else if (lux >= 10000.00) {
    Serial.print("Rovflux");
  } else {
    LoRa.print(lux);
  }
  if (set_lux < 100) {
    LoRa.print("0" + String(set_lux));
  } else {
    LoRa.print(set_lux);
  }
  LoRa.endPacket();
}

int pwm(int x) {
  //pwm function need input 0-100% pwm signal
  int y;
  y = 1023 * x / 100;
  return y;
}
