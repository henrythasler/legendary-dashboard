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
#include <Fonts/FreeMonoBold18pt7b.h>
#include <Fonts/FreeSansBold18pt7b.h>
#include <Fonts/Org_01.h>

// Statistics Helper-Class
#include <statistics.h>
Statistics tempStats(5000U); // ~ 1 week @ 30s sample rate
Statistics humStats(5000U);
Statistics pressStats(5000U);

// uptime calculation
#include <uptime.h>
Uptime uptime;

// Global Variables
float currentTemperatureCelsius = 0;
float currentHumidityPercent = 0;
float currentPressurePascal = 0;

// Track initialisation
uint32_t initStage = 0;

// Flow control, basic task scheduler
#define SCHEDULER_MAIN_LOOP_MS (10) // ms
uint32_t counterBase = 0;
uint32_t counter300s = 0;
uint32_t counter1h = 0;
bool enableDisplay = true;  // display output can be disabled for testing purposes with this flag

/**
 * Sample measurements from environment sensor.
 * 
 ******************************************************/
void doMeasurement(void)
{
  if (environmentSensorAvailable)
  {
    // read current measurements
    currentTemperatureCelsius = bme.readTemperature();
    currentHumidityPercent = bme.readHumidity();
    currentPressurePascal = bme.readPressure() + PRESSURE_MEASUREMENT_CALIBRATION;

    // update statistics for each measurement
    uint32_t timestamp = uptime.getSeconds();
    tempStats.update(timestamp, currentTemperatureCelsius);
    humStats.update(timestamp, currentHumidityPercent);
    pressStats.update(timestamp, currentPressurePascal / 100.); // use hPa
  }
  else
  {
    Serial.println("[  WARN  ] Environment sensor not available.");
  }
}

/**
 * Update the screen
 * 
 ******************************************************/
void updateScreen()
{
  // Temperature Demo
  int offset = (counter300s % 5) * 40;
  display.fillScreen(GxEPD_WHITE);
  display.fillRoundRect(0 + offset, offset, 128, 64, 10, GxEPD_BLACK);
  display.fillRoundRect(2 + offset, 2 + offset, 124, 60, 8, GxEPD_RED);
  display.drawLine(0, 290, 399, 150, GxEPD_BLACK);
  display.drawLine(0, 291, 399, 151, GxEPD_BLACK);

  display.setFont(&FreeMonoBold18pt7b);

  display.setTextColor(GxEPD_BLACK);
  display.setCursor(4 + offset, 40 + offset);
  display.printf("%.1f°C", currentTemperatureCelsius);

  display.setFont(&FreeSansBold18pt7b);

  display.setTextColor(GxEPD_BLACK);
  display.setCursor(4 + offset, 100 + offset);
  display.print("Hello World!");

  // Display stats
  display.setFont(&Org_01);
  display.setCursor(0, 298);
  display.printf("Free: %u KiB (%u KiB)  Temp: %u (%u B)  Hum: %u (%u B) Press: %u (%u B) up: %us",
                 ESP.getFreeHeap() / 1024,
                 ESP.getMaxAllocHeap() / 1024,
                 tempStats.size(),
                 sizeof(tempStats.history) + sizeof(Point) * tempStats.history.capacity(),
                 humStats.size(),
                 sizeof(humStats.history) + sizeof(Point) * humStats.history.capacity(),
                 pressStats.size(),
                 sizeof(pressStats.history) + sizeof(Point) * pressStats.history.capacity(),
                 uptime.getSeconds());
  display.update();
}

/**
 * This is run once on startup
 * 
 ******************************************************/
void setup()
{
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
  initStage++;

  // Setup serial connection for debugging
  Serial.begin(115200U);
  Serial.println();
  Serial.println("[  INIT  ] Begin");
  initStage++;

  Serial.printf("[  INIT  ] ChipRevision: 0x%02X    CpuFreq: %uMHz   FlashChipSize: %uKiB   HeapSize: %uKiB   MAC: %" PRIX64 "   SdkVersion: %s\n",
                ESP.getChipRevision(),
                ESP.getCpuFreqMHz(),
                ESP.getFlashChipSize() / 1024,
                ESP.getHeapSize() / 1024,
                ESP.getEfuseMac(), // FIXME: 2888DEAE114C must be converted to 4c:11:ae:de:88:28
                ESP.getSdkVersion());
  initStage++;

  // Initialize Environment Sensor
  pinMode(BME280_PIN_VCC, OUTPUT);
  digitalWrite(BME280_PIN_VCC, HIGH); // Power-Supply via GPIO (BME280 uses less than 1mA).
  delay(5);                           // wait for BMW280 to power up. Takes around 2ms.
  initStage++;

  if (I2CBME.begin(BME280_PIN_I2C_SDA, BME280_PIN_I2C_SCL, 250000U)) // set I2C-Clock to 250kHz
  {
    initStage++;
    if (bme.begin(BME280_ADDR, &I2CBME)) // use custom Wire-Instance to avoid interference with other libraries.
    {
      initStage++;
      environmentSensorAvailable = true;
      Serial.println("[  INIT  ] found BME280 environment sensor");
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
    Serial.println("[  INIT  ] BME280 setup done");
  }
  else
  {
    Serial.println("[ ERROR  ] Could not find a BME280 sensor, check wiring!");
  }
  initStage++;

  Serial.println("[  INIT  ] setup ePaper display");
  delay(100); // wait a bit, before display-class starts writing to serial out

  display.init(/*115200*/); // uncomment serial speed definition for debug output
  display.setTextColor(GxEPD_BLACK);
  display.setFont(&FreeMonoBold18pt7b);
  display.fillScreen(GxEPD_WHITE);
  delay(100);

  initStage++; // Init complete
  Serial.printf("[  INIT  ] Completed at stage %u\n\n", initStage);
  digitalWrite(LED_BUILTIN, HIGH); // turn on LED to indicate normal operation;
}

/**
 * This runs forever
 * 
 ******************************************************/
void loop()
{
  // 100ms Tasks
  if (!(counterBase % (100L / SCHEDULER_MAIN_LOOP_MS)))
  {
    digitalWrite(LED_BUILTIN, HIGH); // regularly turn on LED
  }

  // 500ms Tasks
  if (!(counterBase % (500L / SCHEDULER_MAIN_LOOP_MS)))
  {
  }

  // 2s Tasks
  if (!(counterBase % (2000L / SCHEDULER_MAIN_LOOP_MS)))
  {
    // indicate alive
    digitalWrite(LED_BUILTIN, LOW);
  }

  // 30s Tasks
  if (!(counterBase % (30000L / SCHEDULER_MAIN_LOOP_MS)))
  {
    doMeasurement();
    // memory state
    Serial.printf("[ MEMORY ] Free: %u KiB (%u KiB)  Temp: %u (%u B)  Hum: %u (%u B) Press: %u (%u B) Uptime: %u s\n",
                  ESP.getFreeHeap() / 1024,
                  ESP.getMaxAllocHeap() / 1024,
                  tempStats.size(),
                  sizeof(tempStats.history) + sizeof(Point) * tempStats.history.capacity(),
                  humStats.size(),
                  sizeof(humStats.history) + sizeof(Point) * humStats.history.capacity(),
                  pressStats.size(),
                  sizeof(pressStats.history) + sizeof(Point) * pressStats.history.capacity(),
                  uptime.getSeconds());
  }

  // 300s Tasks
  // e-Paper Display MUST not be updated more often than every 180s to ensure lifetime function
  if (!(counterBase % (300000L / SCHEDULER_MAIN_LOOP_MS)))
  {
    if (enableDisplay)
    {
      Serial.println("[  DISP  ] Updating...");
      updateScreen();
    }

    counter300s++;
  }

  // 1h Tasks
  if (!(counterBase % (3600000L / SCHEDULER_MAIN_LOOP_MS)))
  {
    if (counter1h > 0) // don't compact on startup
    {
      tempStats.compact(0.2);
      humStats.compact(0.2);
      pressStats.compact(0.2);
    }
    counter1h++;
  }

  delay(SCHEDULER_MAIN_LOOP_MS);
  counterBase++;
}
