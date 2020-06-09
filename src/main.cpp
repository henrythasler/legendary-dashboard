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
#define HAS_RED_COLOR

// Track initialisation
uint32_t initStage = 0;

// Flow control, basic task scheduler
#define SCHEDULER_MAIN_LOOP_MS (10) // ms
uint32_t counterBase = 0;

// run once on startup
void setup()
{
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);
  initStage++;

  // Setup serial connection for debugging
  Serial.begin(115200U);
  Serial.println();
  Serial.println("[ INIT ] - Begin");
  initStage++;

  delay(100);
  display.init(115200);
  display.eraseDisplay(true);
  delay(100);

  initStage++; // Init complete
  Serial.printf("[ INIT ] - Completed at stage %u\n", initStage);
  digitalWrite(LED_BUILTIN, LOW); // turn on LED to indicate normal operation;
}

// run forever
void loop()
{
  // 100ms Tasks
  if (!(counterBase % (100 / SCHEDULER_MAIN_LOOP_MS)))
  {
    digitalWrite(LED_BUILTIN, LOW);
  }

  // 2s Tasks
  if (!(counterBase % (2000 / SCHEDULER_MAIN_LOOP_MS)))
  {
  digitalWrite(LED_BUILTIN, HIGH);
  }

  delay(SCHEDULER_MAIN_LOOP_MS);
  counterBase++;
}
