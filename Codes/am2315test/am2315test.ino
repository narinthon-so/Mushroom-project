#include <Wire.h>
#include <Adafruit_AM2315.h>

// Connect RED of the AM2315 sensor to 5.0V
// Connect BLACK to Ground
// Connect WHITE to i2c clock
// Connect YELLOW to i2c data

Adafruit_AM2315 am2315;

void setup() {
  Serial.begin(115200);
  Wire.begin (21, 22);   // sda= GPIO_21 /scl= GPIO_22

  // Wake up the sensor
  Wire.beginTransmission(AM2315_I2CADDR);
  delay(2);
  Wire.endTransmission();
  
  while (!Serial) {
    delay(10);
  }
  Serial.println("AM2315 Test!");

  if (! am2315.begin()) {
     Serial.println("Sensor not found, check wiring & pullups!");
     while (1);
  }
}

void loop() {
  float temperature, humidity;
  am2315.readTemperatureAndHumidity(&temperature, &humidity);
//  if (! am2315.readTemperatureAndHumidity(&temperature, &humidity)) {
//    Serial.println("Failed to read data from AM2315");
//    return;
//  }
  Serial.print("Temp *C: "); Serial.println(temperature);
  Serial.print("Hum %: "); Serial.println(humidity);

  delay(1000);
}
