#include <Arduino.h>

// Environment sensor includes and defines
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#define BME280_ADDR (0x76)
#define PRESSURE_MEASUREMENT_CALIBRATION (6000)
#define SEALEVEL_PRESSURE (1013.25)
#define SEALEVEL_PRESSURE_CALIBRATION (9.65)

Adafruit_BME280 bme; // I2C

#include <statistics.h>
Statistics tempStats;
Statistics humStats;
Statistics pressStats;

// Global Variables
float currentTemperatureCelsius = 0;
float currentHumidityPercent = 0;
float currentPressurePascal = 0;
int frameCounter = 0;

// run once on startup
void setup()
{
  // Setup serial connection for debugging
  Serial.begin(115200);

  // Initialize Environment Sensor
  if (!bme.begin(BME280_ADDR, &Wire))
  {
    Serial.println("[ ERROR ] - Could not find a BME280 sensor, check wiring!");
    while (1)
      ;
  }

  // Setup Environment Sensor
  bme.setSampling(Adafruit_BME280::MODE_NORMAL,     /* Operating Mode. */
                  Adafruit_BME280::SAMPLING_X2,     /* Temp. oversampling */
                  Adafruit_BME280::SAMPLING_X16,    /* Hum. oversampling */
                  Adafruit_BME280::SAMPLING_X16,    /* Pressure oversampling */
                  Adafruit_BME280::FILTER_X16,      /* Filtering. */
                  Adafruit_BME280::STANDBY_MS_500); /* Standby time. */
}

// run forever
void loop()
{
  frameCounter++;

  // read current measurements
  currentTemperatureCelsius = bme.readTemperature();
  currentHumidityPercent = bme.readHumidity();
  currentPressurePascal = bme.readPressure() + PRESSURE_MEASUREMENT_CALIBRATION;

  // update statistics for each measurement
  tempStats.update(currentTemperatureCelsius);
  humStats.update(currentHumidityPercent);
  pressStats.update(currentPressurePascal / 100.);

  Serial.print("\nTemperature = ");
  Serial.print(currentTemperatureCelsius);
  Serial.print(" °C (▼");
  Serial.print(tempStats.min);
  Serial.print(" ▲");
  Serial.print(tempStats.max);
  Serial.print(")");

  Serial.print("    Humidity = ");
  Serial.print(currentHumidityPercent);
  Serial.print(" % (▼");
  Serial.print(humStats.min);
  Serial.print(" ▲");
  Serial.print(humStats.max);
  Serial.print(")");

  Serial.print("    Pressure = ");
  Serial.print(currentPressurePascal / 100.); // convert to hPa
  Serial.print(" hPa (▼");
  Serial.print(pressStats.min);
  Serial.print(" ▲");
  Serial.print(pressStats.max);
  Serial.print(")");

  Serial.print("    Altitude = ");
  Serial.print(bme.readAltitude(SEALEVEL_PRESSURE + SEALEVEL_PRESSURE_CALIBRATION));
  Serial.print(" m");

  delay(2000);
}