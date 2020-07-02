using namespace std;

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
#define MODEM_RST (5)
#define MODEM_PWKEY (4)
#define MODEM_POWER_ON (23)
#define MODEM_TX (27)
#define MODEM_RX (26)

// Configure TinyGSM library
#define TINY_GSM_MODEM_SIM800     // Modem is SIM800
#define TINY_GSM_RX_BUFFER (1024) // Set RX buffer to 1Kb
#include <TinyGsmClient.h>

// SIM800L includes, defines and variables
#define SerialAT Serial1
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
#define BLACK (0x0000)
#define WHITE (0xFFFF)
#define COLOR (0xF800)
#define HAS_RED_COLOR

// FreeFonts from Adafruit_GFX
#include <Fonts/FreeSansBold9pt7b.h>
#include <Fonts/FreeMonoBold18pt7b.h>
#include <Fonts/FreeSansBold18pt7b.h>
#include <Fonts/FreeSans12pt7b.h>
#include <Fonts/Org_01.h>

// images
#include <images.h>

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

// Command Line Interface
#define LINE_BUF_SIZE 128   //Maximum input string length
#define ARG_BUF_SIZE 64     //Maximum argument string length
#define MAX_NUM_ARGS 8      //Maximum number of arguments
 
bool error_flag = false;
 
char line[LINE_BUF_SIZE];
char args[MAX_NUM_ARGS][ARG_BUF_SIZE];


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


int execute(){  
    Serial.println(line);
    return 0;
}

void read_line(){
    String line_string;
 
    while(!Serial.available());
 
    if(Serial.available()){
        line_string = Serial.readStringUntil('\n');
        if(line_string.length() < LINE_BUF_SIZE){
          line_string.toCharArray(line, LINE_BUF_SIZE);
          Serial.println(line_string);
        }
        else{
          Serial.println("Input string too long.");
          error_flag = true;
        }
    }
}
 
void parse_line(){
    char *argument;
    int counter = 0;
 
    argument = strtok(line, " ");
 
    while((argument != NULL)){
        if(counter < MAX_NUM_ARGS){
            if(strlen(argument) < ARG_BUF_SIZE){
                strcpy(args[counter],argument);
                argument = strtok(NULL, " ");
                counter++;
            }
            else{
                Serial.println("Input string too long.");
                error_flag = true;
                break;
            }
        }
        else{
            break;
        }
    }
}

void prompt(void)
{
  Serial.print("> ");

  read_line();
  if (!error_flag)
  {
    parse_line();
  }
  if (!error_flag)
  {
    execute();
  }

  memset(line, 0, LINE_BUF_SIZE);
  memset(args, 0, sizeof(args[0][0]) * MAX_NUM_ARGS * ARG_BUF_SIZE);

  error_flag = false;
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
                __builtin_bswap64(ESP.getEfuseMac()) >> 16, // swap endianess and rshift for better readability
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
  io.setFrequency(500000L); // set to 500kHz; the default setting (4MHz) could be unreliable with active modem and unshielded wiring
  display.setTextColor(BLACK);
  display.setFont(&FreeMonoBold18pt7b);
  display.fillScreen(WHITE);
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
  prompt();
}
