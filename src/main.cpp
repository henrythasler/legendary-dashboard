#include <Arduino.h>

#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

Adafruit_BME280 bme; // I2C
#define BME280_ADDR 0x76

void setup() {
  Serial.begin(115200);

  if (!bme.begin(BME280_ADDR, &Wire))
  {
    Serial.println("[ ERROR ] - Could not find a BME280 sensor, check wiring!");
    while (1)
      ;
  }
}

void loop() {
  Serial.print("Temperature = ");
  Serial.print(bme.readTemperature());
  Serial.println(" °C");

  delay(2000);
}