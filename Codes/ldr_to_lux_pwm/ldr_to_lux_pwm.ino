// These constants, define values needed for the LDR readings and ADC
#define LDR_PIN                   25
#define MAX_ADC_READING           4095
#define ADC_REF_VOLTAGE           3.3
#define REF_RESISTANCE            5030  // measure this for best results
#define LUX_CALC_SCALAR           12518931
#define LUX_CALC_EXPONENT         -1.405
// the number of the LED pin
const int ledPin = 2;  // 16 corresponds to GPIO16

// setting PWM properties
const int freq = 547;
const int ledChannel = 0;
const int resolution = 10;
int pwmvalue;

void setup(void)
{
  Serial.begin(115200);
  // configure LED PWM functionalitites
  ledcSetup(ledChannel, freq, resolution);

  // attach the channel to the GPIO to be controlled
  ledcAttachPin(ledPin, ledChannel);

  Serial.println(F("Light Sensor Test")); Serial.println("");

  /* We're ready to go! */
  Serial.println(F("Sensor, LCD and SD Card are Configured."));
}

/**************************************************************************/
void loop(void)
{

  int   ldrRawData;
  float resistorVoltage, ldrVoltage;
  float ldrResistance;
  float ldrLux;

  // Perform the analog to digital conversion
  ldrRawData = analogRead(LDR_PIN);

  // RESISTOR VOLTAGE_CONVERSION
  // Convert the raw digital data back to the voltage that was measured on the analog pin
  resistorVoltage = (float)ldrRawData / MAX_ADC_READING * ADC_REF_VOLTAGE;

  // voltage across the LDR is the 5V supply minus the 5k resistor voltage
  ldrVoltage = ADC_REF_VOLTAGE - resistorVoltage;

  // LDR_RESISTANCE_CONVERSION
  // resistance that the LDR would have for that voltage
  ldrResistance = ldrVoltage / resistorVoltage * REF_RESISTANCE;

  // LDR_LUX
  // Change the code below to the proper conversion from ldrResistance to
  // ldrLux
  ldrLux = LUX_CALC_SCALAR * pow(ldrResistance, LUX_CALC_EXPONENT);

  if (ldrLux < 200) {
    if (pwmvalue < 100) {
      pwmvalue++;
    }
    if (pwmvalue == 100) {
      pwmvalue = 100;
    }
  }
  else if (ldrLux > 200) {
    if (pwmvalue > 0) {
      pwmvalue--;
    }
    if (pwmvalue == 0) {
      pwmvalue = 0;
    }
  }
  ledcWrite(ledChannel, pwm(pwmvalue));

  // print out the results
  Serial.print("LDR Raw Data   : "); Serial.println(ldrRawData);
  Serial.print("LDR Voltage    : "); Serial.print(ldrVoltage); Serial.println(" volts");
  Serial.print("LDR Resistance : "); Serial.print(ldrResistance); Serial.println(" Ohms");
  Serial.print("LDR Illuminance: "); Serial.print(ldrLux); Serial.println(" lux");
  Serial.println(pwmvalue);
  //  // Print out the LDR Illuminance to the LCD
  //  lcd.setCursor(0,0); // set cursor to column 0, line 1
  //  lcd.print("LDR Lux Meter");
  //
  //  lcd.setCursor(0,1); // set cursor to column 0, line 2
  //  lcd.print(ldrLux);
  //  lcd.setCursor(7, 1); // set cursor to column 4, line 1
  //  lcd.print(" lux");

  delay(250);
}

int pwm(int x) {
  //pwm function need input 0-100% pwm signal
  int y;
  y = 1023 * x / 100;
  return y;
}
