using namespace std;

#include <Arduino.h>
#include <Wire.h>

#define LED_BUILTIN (13) // LED is connected to IO13

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

// images
#include <images.h>
#include <vector>
vector<Image> slideshow;

// uptime calculation
#include <uptime.h>
Uptime uptime;

// Track initialisation
uint32_t initStage = 0;

// Flow control, basic task scheduler
#define SCHEDULER_MAIN_LOOP_MS (10) // ms
uint32_t counterBase = 0;
uint32_t counter300s = 0;
uint32_t counter1h = 0;
bool enableDisplay = true; // display output can be disabled for testing purposes with this flag

/**
 * Update the screen
 * 
 ******************************************************/
void updateScreen()
{
  // Slideshow
  display.drawBitmap(slideshow.at(counter300s % slideshow.size()).color, 0, 0, 400, 400, COLOR, display.bm_invert);
  display.drawBitmap(slideshow.at(counter300s % slideshow.size()).black, 0, 0, 400, 300, BLACK, display.bm_invert | display.bm_transparent);
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
                __builtin_bswap64(ESP.getEfuseMac()) >> 16, // swap endianess and rshift for better readability
                ESP.getSdkVersion());
  initStage++;

  Serial.println("[  INIT  ] setup ePaper display");
  delay(100); // wait a bit, before display-class starts writing to serial out

  display.init(/*115200*/); // uncomment serial speed definition for debug output
  io.setFrequency(500000L); // set to 500kHz; the default setting (4MHz) could be unreliable with active modem and unshielded wiring
  delay(100);
  initStage++;

  // setup slideshow content
  Serial.println("[  INIT  ] Creating slideshow...");
  slideshow.push_back(images.beer);
  slideshow.push_back(images.bmw);
  slideshow.push_back(images.bruce);
  slideshow.push_back(images.yellowScreen);
  slideshow.push_back(images.coffin);
  slideshow.push_back(images.parking);
  slideshow.push_back(images.unittest);
  slideshow.push_back(images.fixing);
  initStage++;

  Serial.println("[  INIT  ] Clock synchronization");
  // updateModemInfo();
  uptime.setTime({tm_sec : 0, tm_min : 00, tm_hour : 0, tm_mday : 1, tm_mon : 01, tm_year : 2020 - 1900});
  setenv("TZ", "CET-1CEST,M3.5.0,M10.5.0", 1); //"Europe/Berlin"  from: http://www.famschmid.net/timezones.html
  tzset();                                     // Assign the local timezone from setenv

  tm *tm = uptime.getTime();
  Serial.printf("[  INIT  ] Current time is: %02d.%02d.%04d %02d:%02d:%02d\n", tm->tm_mday, tm->tm_mon, tm->tm_year + 1900, tm->tm_hour, tm->tm_min, tm->tm_sec);
  initStage++;

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

  // 2s Tasks
  if (!(counterBase % (500L / SCHEDULER_MAIN_LOOP_MS)))
  {
    digitalWrite(LED_BUILTIN, LOW);
  }

  // 60s Tasks
  if (!(counterBase % (60000L / SCHEDULER_MAIN_LOOP_MS)))
  {
    Serial.print("[  DISP  ] Updating... ");
    updateScreen();
    Serial.println("ok");
    counter300s++;
  }

  delay(SCHEDULER_MAIN_LOOP_MS);
  counterBase++;
}
