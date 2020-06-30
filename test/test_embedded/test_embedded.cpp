#include <Arduino.h>
#include <unity.h>

#define LED_BUILTIN (13) // LED is connected to IO13

// void setUp(void) {
// // set stuff up here
// }

// void tearDown(void) {
// // clean stuff up here
// }

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
#define SerialMon Serial
#define SerialAT Serial1
// const char simPIN[]   = "1234"; // SIM card PIN code, if any
#define TINY_GSM_DEBUG SerialMon

TinyGsm modem(SerialAT);

void test_modem_connection(void)
{
    // bool init = modem.init();
    // TEST_ASSERT_EQUAL(init, true);

    String name = modem.getModemName();
    TEST_ASSERT_NOT_NULL(name);

    // modem.gprsConnect(apn, NULL, NULL);
    if (!modem.waitForNetwork(60000L)) {
        Serial.write("nope, no network...");
        TEST_FAIL_MESSAGE("no network");
        return;
    }
    TEST_ASSERT_TRUE(modem.isNetworkConnected());
}


void test_led_builtin_pin_number(void)
{
    TEST_ASSERT_EQUAL(LED_BUILTIN, 13);
}

void test_led_state_high(void)
{
    digitalWrite(LED_BUILTIN, HIGH);
    TEST_ASSERT_EQUAL(digitalRead(LED_BUILTIN), HIGH);
}

void test_led_state_low(void)
{
    digitalWrite(LED_BUILTIN, LOW);
    TEST_ASSERT_EQUAL(digitalRead(LED_BUILTIN), LOW);
}

void setup()
{
    UNITY_BEGIN();
    RUN_TEST(test_led_builtin_pin_number);
    
    // prepare for I/O test
    pinMode(LED_BUILTIN, OUTPUT);

    RUN_TEST(test_led_state_high);
    RUN_TEST(test_led_state_low);    

    // reset modem, just for the lulz
    digitalWrite(MODEM_RST, HIGH);
    delay(5000);
    digitalWrite(MODEM_RST, LOW);
    delay(1300);
    digitalWrite(MODEM_RST, HIGH);

    RUN_TEST(test_modem_connection);

    UNITY_END();
}

void loop()
{
    digitalWrite(LED_BUILTIN, HIGH);
    delay(250);
    digitalWrite(LED_BUILTIN, LOW);
    delay(250);    
}
