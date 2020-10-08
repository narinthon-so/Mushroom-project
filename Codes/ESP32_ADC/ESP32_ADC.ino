#define R_sensing_pump 25
#define R_sensing_fan 35

int adc_pump_value, adc_fan_value = 0;
float Rs_pump_voltage, Rs_fan_voltage = 0;

void setup() {
  Serial.begin(115200);
  analogReadResolution(12);
}

void loop() {
  // Reading adc values
  adc_pump_value = analogRead(R_sensing_pump);
  adc_fan_value = analogRead(R_sensing_fan);
  Serial.print(adc_pump_value); Serial.print("  "); Serial.println(adc_fan_value);

  //calculate to voltage
  Rs_pump_voltage = (adc_pump_value * 3.3 / 4095);
  Rs_fan_voltage = (adc_fan_value * 3.3 / 4095);
  Serial.print(Rs_pump_voltage); Serial.print("  "); Serial.println(Rs_fan_voltage);

  delay(300);
}
