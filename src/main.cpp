#include <Arduino.h>

#define LED_BUILTIN (13) // LED is connected to IO13

// Environment sensor includes and defines
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#define BME280_PIN_I2C_SCL (32)
#define BME280_PIN_I2C_SDA (33)
#define BME280_PIN_VCC (25)
#define BME280_ADDR (0x76)
#define PRESSURE_MEASUREMENT_CALIBRATION (6000)
#define SEALEVEL_PRESSURE (1013.25)
#define SEALEVEL_PRESSURE_CALIBRATION (9.65)

TwoWire I2CBME = TwoWire(0); // set up a new Wire-Instance for BME280 Environment Sensor
Adafruit_BME280 bme;         // use I2C
bool environmentSensorAvailable = false;

// Display stuff
#include <GxEPD.h>
#include <GxGDEW042Z15/GxGDEW042Z15.h> // 4.2" b/w/r
#include <GxIO/GxIO_SPI/GxIO_SPI.h>
#include <GxIO/GxIO.h>

// platformIO board definition: MOSI=15 SCK=14
GxIO_Class io(SPI, /*CS*/ 19, /*DC*/ 18, /*RST*/ 0);
GxEPD_Class display(io, /*RST*/ 0, /*BUSY*/ 2);
#define HAS_RED_COLOR

// FreeFonts from Adafruit_GFX
#include <Fonts/FreeMonoBold9pt7b.h>
#include <Fonts/FreeMonoBold12pt7b.h>
#include <Fonts/FreeMonoBold18pt7b.h>
#include <Fonts/FreeMonoBold24pt7b.h>
#include <Fonts/FreeSans18pt7b.h>

// Statistics Helper-Class
#include <statistics.h>
Statistics tempStats;
Statistics humStats;
Statistics pressStats;

// Global Variables
float currentTemperatureCelsius = 0;
float currentHumidityPercent = 0;
float currentPressurePascal = 0;

// Track initialisation
uint32_t initStage = 0;

// Flow control, basic task scheduler
#define SCHEDULER_MAIN_LOOP_MS (10) // ms
uint32_t counterBase = 0;
uint32_t counter30s = 0;

// run once on startup
void setup()
{
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
  initStage++;

  // Setup serial connection for debugging
  Serial.begin(115200U);
  Serial.println();
  Serial.println("[ INIT ] - Begin");
  initStage++;

  // Initialize Environment Sensor
  pinMode(BME280_PIN_VCC, OUTPUT);
  digitalWrite(BME280_PIN_VCC, HIGH); // Power-Supply via GPIO (BME280 uses less than 1mA).
  delay(5);                           // wait for BMW280 to power up. Takes around 2ms.

  if (I2CBME.begin(BME280_PIN_I2C_SDA, BME280_PIN_I2C_SCL, 250000U)) // set I2C-Clock to 250kHz
  {
    initStage++;
    if (bme.begin(BME280_ADDR, &I2CBME)) // use custom Wire-Instance to avoid interference with other libraries.
    {
      initStage++;
      environmentSensorAvailable = true;
    }
  }

  if (environmentSensorAvailable)
  {
    initStage++;
    // Setup Environment Sensor
    bme.setSampling(Adafruit_BME280::MODE_NORMAL,     /* Operating Mode. */
                    Adafruit_BME280::SAMPLING_X2,     /* Temp. oversampling */
                    Adafruit_BME280::SAMPLING_X16,    /* Hum. oversampling */
                    Adafruit_BME280::SAMPLING_X16,    /* Pressure oversampling */
                    Adafruit_BME280::FILTER_X16,      /* Filtering. */
                    Adafruit_BME280::STANDBY_MS_500); /* Standby time. */
  }
  else
  {
    Serial.println("[ ERROR ] - Could not find a BME280 sensor, check wiring!");
  }
  initStage++;

  delay(100);
  display.init(115200);
  display.setTextColor(GxEPD_BLACK);
  display.setFont(&FreeMonoBold18pt7b);
  display.fillScreen(GxEPD_WHITE);
  display.eraseDisplay(true);
  delay(100);

  initStage++; // Init complete
  Serial.print("[ INIT ] - Completed at stage ");
  Serial.println(initStage);
  Serial.println();
  digitalWrite(LED_BUILTIN, HIGH); // turn on LED;
}

// run forever
void loop()
{
  counterBase++;

  // 100ms Tasks
  if (!(counterBase % (100 / SCHEDULER_MAIN_LOOP_MS)))
  {
    digitalWrite(LED_BUILTIN, HIGH);
  }

  // 500ms Tasks
  if (!(counterBase % (500 / SCHEDULER_MAIN_LOOP_MS)))
  {
  }

  // 2s Tasks
  if (!(counterBase % (2000 / SCHEDULER_MAIN_LOOP_MS)))
  {
    if (environmentSensorAvailable)
    {
      // indicate measurement
      digitalWrite(LED_BUILTIN, LOW);

      // read current measurements
      currentTemperatureCelsius = bme.readTemperature();
      currentHumidityPercent = bme.readHumidity();
      currentPressurePascal = bme.readPressure() + PRESSURE_MEASUREMENT_CALIBRATION;

      // update statistics for each measurement
      tempStats.update(currentTemperatureCelsius);
      humStats.update(currentHumidityPercent);
      pressStats.update(currentPressurePascal / 100.); // use hPa

      Serial.print("Temperature = ");
      Serial.print(currentTemperatureCelsius);
      Serial.print(" °C");

      Serial.print("    Humidity = ");
      Serial.print(currentHumidityPercent);
      Serial.print(" %");

      Serial.print("    Pressure = ");
      Serial.print(currentPressurePascal / 100.); // convert to hPa
      Serial.print(" hPa");

      Serial.print("    Altitude = ");
      Serial.print(bme.readAltitude(SEALEVEL_PRESSURE + SEALEVEL_PRESSURE_CALIBRATION));
      Serial.println(" m");
    }
    else
    {
      Serial.println("Environment sensor not available.");
    }
  }

  // 180s Tasks
  if (!(counterBase % (180000L / SCHEDULER_MAIN_LOOP_MS)))
  {
    counter30s++;
    display.fillRoundRect(0, 0, 128, 64, 10, GxEPD_BLACK);
    display.fillRoundRect(2, 2, 124, 60, 8, GxEPD_RED);
    display.setCursor(4, 40);
    display.setFont(&FreeMonoBold18pt7b);
    display.printf("%.1f°C", currentTemperatureCelsius);
    display.setFont(&FreeSans18pt7b);
    display.setCursor(4, 100);
    display.print("Hallo Huy!");
    display.update();
  }

  delay(SCHEDULER_MAIN_LOOP_MS);
}
