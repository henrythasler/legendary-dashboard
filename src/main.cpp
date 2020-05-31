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

  bme.getTemperatureSensor()->printSensorDetails();
  bme.getHumiditySensor()->printSensorDetails();
  bme.getPressureSensor()->printSensorDetails();
}

void loop() {
  Serial.print("\nTemperature = ");
  Serial.print(bme.readTemperature());
  Serial.print(" °C");

  Serial.print("    Humidity = ");
  Serial.print(bme.readHumidity());
  Serial.print(" %");

  Serial.print("    Pressure = ");
  Serial.print(bme.readPressure()/100);
  Serial.print(" hPa");

  delay(2000);
}