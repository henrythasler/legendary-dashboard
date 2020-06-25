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

// Modem pinning
#define MODEM_RST            (5)
#define MODEM_PWKEY          (4)
#define MODEM_POWER_ON       (23)
#define MODEM_TX             (27)
#define MODEM_RX             (26)

// Configure TinyGSM library
#define TINY_GSM_MODEM_SIM800        // Modem is SIM800
#define TINY_GSM_RX_BUFFER   (1024)  // Set RX buffer to 1Kb
#include <TinyGsmClient.h>

// SIM800L includes, defines and variables
#define SerialAT  Serial1
// const char simPIN[]   = "1234"; // SIM card PIN code, if any
TinyGsm modem(SerialAT);

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
#include <Fonts/FreeSansBold9pt7b.h>
#include <Fonts/FreeMonoBold18pt7b.h>
#include <Fonts/FreeSansBold18pt7b.h>
#include <Fonts/Org_01.h>

// Statistics Helper-Class
#include <timeseries.h>
Timeseries tempStats(5000U);
Timeseries humStats(5000U);
Timeseries pressStats(5000U);

// uptime calculation
#include <uptime.h>
Uptime uptime;

// charts
#include <chart.h>
Chart chart;

// Global Variables
float currentTemperatureCelsius = 0;
float currentHumidityPercent = 0;
float currentPressurePascal = 0;

uint8_t currentSignalStrength = 0;
int currentYear = 0;
int currentMonth = 0;
int currentDay = 0;
int currentHour = 0;
int currentMin = 0;
int currentSec = 0;
float currentTimezone = 0.0;


// Track initialisation
uint32_t initStage = 0;

// Flow control, basic task scheduler
#define SCHEDULER_MAIN_LOOP_MS (10) // ms
uint32_t counterBase = 0;
uint32_t counter300s = 0;
uint32_t counter1h = 0;
bool enableDisplay = true; // display output can be disabled for testing purposes with this flag

/**
 * Update all information from modem.
 * 
 ******************************************************/
void updateModemInfo(void)
{
    // read current data
    currentSignalStrength = modem.getSignalQuality();
    Serial.print("[ MODEM  ] Sigal Quality [0-31]: ");
    Serial.println(currentSignalStrength);

  if (modem.isNetworkConnected())
  {
    modem.getNetworkTime(&currentYear, &currentMonth, &currentDay, &currentHour, &currentMin, &currentSec, &currentTimezone);
    Serial.printf("[ MODEM  ] Current Network Time (Values) - Year: %d, Month: %02d, Day: %02d, Hour: %02d, Minute: %02d, Second: %02d, Timezone: %.1f\n",
        currentYear, currentMonth, currentDay, currentHour, currentMin, currentSec, currentTimezone);
  }
  else
  {
    Serial.println("[  WARN  ] Network not connected.");
  }

}

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
    tempStats.push(timestamp, currentTemperatureCelsius);
    humStats.push(timestamp, currentHumidityPercent);
    pressStats.push(timestamp, currentPressurePascal / 100.); // use hPa
  }
  else
  {
    Serial.println("[  WARN  ] Environment sensor not available.");
  }
}

/**
 * Draw bars to show signal strength of mobile network
 * 
 ******************************************************/
void drawSignalStrength(int strength, int x, int y, int numbars, int barwidth, int barheight, int heightdelta, int gap)
{
  int i;

  for (i=0; i<numbars; i++) {

    if ( strength > (int) ((31 / (numbars + 1))) * (i+1) ) {
      display.fillRect( x + i * (barwidth + gap), y + (numbars - 1- i) * heightdelta, barwidth, barheight - (numbars - 1 - i) * heightdelta, GxEPD_RED );
    }

    display.drawRect( x + i * (barwidth + gap), y + (numbars - 1- i) * heightdelta, barwidth, barheight - (numbars - 1 - i) * heightdelta, GxEPD_BLACK );
  }
}

/**
 * Update the screen
 * 
 ******************************************************/
void updateScreen()
{
  // Temperature Demo
  display.fillScreen(GxEPD_WHITE);
  display.fillRoundRect(5, 5, 138, 64, 10, GxEPD_BLACK);
  display.fillRoundRect(7, 7, 134, 60, 8, GxEPD_RED);

  display.setFont(&FreeMonoBold18pt7b);

  display.setTextColor(GxEPD_WHITE);
  display.setCursor(10, 45);
  display.printf("%.1f C", currentTemperatureCelsius);

  // Date
  display.setFont(&FreeSansBold18pt7b);
  display.setTextColor(GxEPD_RED);
  display.setCursor(160, 45);
  if (currentYear == 0)
    display.printf("--.--.----");
  else
    display.printf("%02d.%02d.%04d", currentDay, currentMonth, currentYear);

  // Udate Time
  display.setFont(&FreeSansBold9pt7b);
  display.setTextColor(GxEPD_BLACK);
  display.setCursor(160, 65);
  if (currentYear == 0)
    display.printf("Last updated: --:--:--");
  else
    display.printf("Last updated: %02d:%02d:%02d", currentHour, currentMin, currentSec);
  
  // Signal strength
  drawSignalStrength( currentSignalStrength, 345, 10, 5, 7, 40, 5, 3);

  // Uptime and Memory stats
  display.setFont(&Org_01);
  display.setTextColor(GxEPD_BLACK);
  display.setCursor(0, 290);
  display.printf("up: %us\nFree: %u KiB (%u KiB)  Temp: %u (%u B)  Hum: %u (%u B) Press: %u (%u B)",
                 uptime.getSeconds(),
                 ESP.getFreeHeap() / 1024,
                 ESP.getMaxAllocHeap() / 1024,
                 tempStats.size(),
                 sizeof(tempStats.data) + sizeof(Point) * tempStats.data.capacity(),
                 humStats.size(),
                 sizeof(humStats.data) + sizeof(Point) * humStats.data.capacity(),
                 pressStats.size(),
                 sizeof(pressStats.data) + sizeof(Point) * pressStats.data.capacity());

  // Linecharts
  // Chart Title
  display.setFont(&FreeSansBold9pt7b);
  display.setTextColor(GxEPD_BLACK);
  display.setCursor(10, 145);
  display.print("Temp");
  display.setCursor(145, 145);
  display.print("Hum");
  display.setCursor(280, 145);
  display.print("Press");

  // Y-Axis Labels
  display.setFont(&Org_01);
  display.setTextColor(GxEPD_BLACK);
  display.setCursor(2, 155);
  display.printf("%.1f", tempStats.max);
  display.setCursor(2, 249);
  display.printf("%.1f", tempStats.min);

  display.setCursor(135, 155);
  display.printf("%.0f", humStats.max);
  display.setCursor(135, 249);
  display.printf("%.0f", humStats.min);

  display.setCursor(268, 155);
  display.printf("%.1f", pressStats.max);
  display.setCursor(268, 249);
  display.printf("%.1f", pressStats.min);

  // Frame
  display.drawFastHLine(0, 149, 400, GxEPD_BLACK);
  display.drawFastHLine(0, 251, 400, GxEPD_BLACK);
  display.drawFastVLine(133, 149, 102, GxEPD_BLACK);
  display.drawFastVLine(266, 149, 102, GxEPD_BLACK);

  // Charts
  chart.lineChart(&display, &tempStats, 0, 150, 130, 100, GxEPD_RED);
  chart.lineChart(&display, &humStats, 135, 150, 130, 100, GxEPD_BLACK);
  chart.lineChart(&display, &pressStats, 270, 150, 130, 100, GxEPD_BLACK);


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
  io.setFrequency(500000L); // set to 500kHz; the default 4MHz-setting is unreliable with active modem and unshielded wiring
  display.setTextColor(GxEPD_BLACK);
  display.setFont(&FreeMonoBold18pt7b);
  display.fillScreen(GxEPD_WHITE);
  delay(100);
  initStage++;

  // Initialize SIM800L Module
  // Set-up modem reset, enable, power pins
  Serial.println("[  INIT  ] modem power-on");
  pinMode(MODEM_PWKEY, OUTPUT);
  pinMode(MODEM_RST, OUTPUT);
  pinMode(MODEM_POWER_ON, OUTPUT);
  digitalWrite(MODEM_PWKEY, LOW);
  digitalWrite(MODEM_RST, HIGH);
  digitalWrite(MODEM_POWER_ON, HIGH);

  // Set GSM module baud rate and UART pins
  Serial.println("[  INIT  ] Initializing serial interface to modem...");
  SerialAT.begin(115200, SERIAL_8N1, MODEM_RX, MODEM_TX);
  initStage++;

  // Restart takes quite some time
  // use modem.init() if you don't need the complete restart
  Serial.println("[  INIT  ] Initializing modem...");
  modem.restart();
  
  String name = modem.getModemName();
  Serial.print("[  INIT  ] Modem Name: ");
  Serial.print(name);
  String modemInfo = modem.getModemInfo();
  Serial.print(", Modem Info: ");
  Serial.println(modemInfo);
  initStage++;

  // Unlock your SIM card with a PIN if needed
  // if (strlen(simPIN) && modem.getSimStatus() != 3 ) {
  //   Serial.println("Unlocking SIM...");
  //   modem.simUnlock(simPIN);
  // }
  
  initStage++; // Init complete
  Serial.printf("[  INIT  ] Completed at stage %u\n\n", initStage);
  digitalWrite(LED_BUILTIN, HIGH); // turn on LED to indicate normal operation;

  // delay to allow modem connect to network
  delay(4000);
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
                  sizeof(tempStats.data) + sizeof(Point) * tempStats.data.capacity(),
                  humStats.size(),
                  sizeof(humStats.data) + sizeof(Point) * humStats.data.capacity(),
                  pressStats.size(),
                  sizeof(pressStats.data) + sizeof(Point) * pressStats.data.capacity(),
                  uptime.getSeconds());  
  }

  // 300s Tasks
  // e-Paper Display MUST not be updated more often than every 180s to ensure lifetime function
  if (!(counterBase % (300000L / SCHEDULER_MAIN_LOOP_MS)))
  {
    if (counter300s > 0) // don't trim/compact on startup
    {
      // RAM is limited so we cut off the timeseries after x days
      tempStats.trim(uptime.getSeconds(), 7 * 24 * 3600);
      humStats.trim(uptime.getSeconds(), 7 * 24 * 3600);
      pressStats.trim(uptime.getSeconds(), 7 * 24 * 3600);

      // and apply compression (Ramer-Douglas-Peucker)
      tempStats.compact(0.05);
      humStats.compact(0.2);
      pressStats.compact(0.05);
    }

    // update modem Information every time display is updated, to get current timestamp
    updateModemInfo();
  
    if (enableDisplay)
    {
      Serial.print("[  DISP  ] Updating... ");
      updateScreen();
      Serial.println("ok");
    }

    counter300s++;
  }

  // 1h Tasks
  if (!(counterBase % (3600000L / SCHEDULER_MAIN_LOOP_MS)))
  {
    counter1h++;
  }

  delay(SCHEDULER_MAIN_LOOP_MS);
  counterBase++;
}
