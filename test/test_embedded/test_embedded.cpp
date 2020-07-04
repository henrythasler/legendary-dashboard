#include <Arduino.h>
#include <unity.h>

#define LED_BUILTIN (13) // LED is connected to IO13

// Modem serial port
#define SerialAT Serial1

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

// void setUp(void) {
// // set stuff up here
// }

// void tearDown(void) {
// // clean stuff up here
// }

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

TinyGsm modem(SerialAT);

void test_modem(void)
{
    // reset modem, just for the lulz
    Serial.println("[  INIT  ] modem power-on");
    pinMode(MODEM_PWKEY, OUTPUT);
    pinMode(MODEM_RST, OUTPUT);
    pinMode(MODEM_POWER_ON, OUTPUT);
    digitalWrite(MODEM_PWKEY, LOW);
    delay(1000);
    digitalWrite(MODEM_RST, HIGH);
    delay(1000);
    digitalWrite(MODEM_POWER_ON, HIGH);
    delay(6000);

    // prepare modem for test
    Serial.println("[  INIT  ] init serial ifc to modem...");
    SerialAT.begin(115200, SERIAL_8N1, MODEM_RX, MODEM_TX);

    modem.streamClear();

    String res = modem.stream.readString();
    Serial.print("[  TEST  ] response: ");
    Serial.println(res);

    modem.sendAT(GF("+CIPGSMLOC=1,1"));
    int code = modem.waitResponse(5000L, "+CIPGSMLOC: ");
    Serial.print("[  TEST  ] response code of CIPGSMLOC: ");
    Serial.println(code);
    TEST_ASSERT_GREATER_OR_EQUAL_INT_MESSAGE(1, code, "woke clock orange");
    
    res = modem.stream.readString();
    Serial.print("[  TEST  ] 1st string: ");
    Serial.println(res);

    TEST_ASSERT_NOT_NULL_MESSAGE(res, "1st read failed");
    modem.waitResponse(); // wait for the OK

    modem.sendAT(GF("+CCLK?"));
    code = modem.waitResponse(5000L, "+CCLK: ");
    Serial.print("[  TEST  ] response code of CCLK: ");
    Serial.println(code);
    TEST_ASSERT_GREATER_OR_EQUAL_INT_MESSAGE(1, code, "woke clock orange");
    
    res = modem.stream.readString();
    Serial.print("[  TEST  ] 2nd string: ");
    Serial.println(res);

    TEST_ASSERT_NOT_NULL_MESSAGE(res, "2nd read failed");

    modem.waitResponse(); // wait for the OK
}

void setup()
{
    UNITY_BEGIN();
    RUN_TEST(test_led_builtin_pin_number);
    
    // prepare for I/O test
    pinMode(LED_BUILTIN, OUTPUT);

    RUN_TEST(test_led_state_high);
    RUN_TEST(test_led_state_low);    

    // modem related stuff
    RUN_TEST(test_modem);

    UNITY_END();
}

void loop()
{
    digitalWrite(LED_BUILTIN, HIGH);
    delay(250);
    digitalWrite(LED_BUILTIN, LOW);
    delay(250);    
}