#include <Arduino.h>

// Environment sensor includes and defines
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#define I2C_SDA (21)
#define I2C_SCL (22)
#define BME280_ADDR (0x76)
#define PRESSURE_MEASUREMENT_CALIBRATION (6000)
#define SEALEVEL_PRESSURE (1013.25)
#define SEALEVEL_PRESSURE_CALIBRATION (9.65)

TwoWire I2CBME = TwoWire(0);
Adafruit_BME280 bme; // use I2C
bool environmentSensorAvailable = true;

// Statistics Helper-Class
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
  I2CBME.begin(I2C_SDA, I2C_SCL, 100000); // Set I2C-Speed manually to lower frequency to avoid communication issues with sensor.
  if (!bme.begin(BME280_ADDR, &I2CBME))
  {
    Serial.println("[ ERROR ] - Could not find a BME280 sensor, check wiring!");
    environmentSensorAvailable = false;
  }

  if (environmentSensorAvailable)
  {
    // Setup Environment Sensor
    bme.setSampling(Adafruit_BME280::MODE_NORMAL,     /* Operating Mode. */
                    Adafruit_BME280::SAMPLING_X2,     /* Temp. oversampling */
                    Adafruit_BME280::SAMPLING_X16,    /* Hum. oversampling */
                    Adafruit_BME280::SAMPLING_X16,    /* Pressure oversampling */
                    Adafruit_BME280::FILTER_X16,      /* Filtering. */
                    Adafruit_BME280::STANDBY_MS_500); /* Standby time. */
  }
}

// run forever
void loop()
{
  frameCounter++;

  if (environmentSensorAvailable)
  {

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
  }
  else
  {
    Serial.println("Environment sensor not available.");
  }
  delay(2000);
}