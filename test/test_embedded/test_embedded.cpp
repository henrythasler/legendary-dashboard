#include <Arduino.h>
#include <ArduinoHttpClient.h>
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
#define TINY_GSM_DEBUG SerialMon

TinyGsm modem(SerialAT);

#define GSM_PIN "148348"

void test_modem_connection(void)
{
    Serial.println("[  TEST  ] modem test");

    // Set GSM module baud rate and UART pins
    Serial.println("[ TEST  ] Initializing serial interface to modem...");
    SerialAT.begin(115200, SERIAL_8N1, MODEM_RX, MODEM_TX);
    
    // TEST_ASSERT_TRUE_MESSAGE(modem.begin(GSM_PIN), "be gune");
    // TEST_ASSERT_TRUE_MESSAGE(modem.init(GSM_PIN), "init good");    

    // if ( modem.getSimStatus() != 3 ) {
    //      Serial.println("Unlock SIM");
    //      bool sim = modem.simUnlock(GSM_PIN);
    //      TEST_ASSERT_TRUE_MESSAGE(sim, "GSM access all areas");    
    // }

    String name = modem.getModemName();
    TEST_ASSERT_NOT_NULL_MESSAGE(name, "it has a name");
    Serial.print(name);
    Serial.println(" was its name-o");

    modem.sendAT(GF("+CCLK?"));
    TEST_ASSERT_EQUAL_MESSAGE(modem.waitResponse(2000L), 1, "woke clock orange");

    String timedate = modem.stream.readString(); 
    Serial.print("[ TEST  ] no matter what year, for me it's always ");
    Serial.println(timedate);
    modem.waitResponse(); 

    bool connect = modem.gprsConnect("iot.1nce.net");
    TEST_ASSERT_TRUE_MESSAGE(connect, "connect GPRS");

    bool network = modem.waitForNetwork(60000L);
    TEST_ASSERT_TRUE_MESSAGE(network, "did network");
    TEST_ASSERT_TRUE_MESSAGE(modem.isNetworkConnected(), "do has network");
}

TinyGsmClient client(modem);
HttpClient http(client, "worldtimeapi.org", 80);

void test_world_wide_wait(void) {

    Serial.println("[  TEST  ] web test");
    Serial.println("[  TEST  ] worldtimeapi.org:80/api/timezone/Europe/Berlin");

    int err = http.get("/api/timezone/Europe/Berlin");
    TEST_ASSERT_EQUAL_MESSAGE(0, err, "no server conn");

    int length = http.contentLength();
    TEST_ASSERT_GREATER_THAN_MESSAGE(0, length, "no content");
    Serial.printf("[  TEST  ] recvd %d bytes\n", length);

    int responseCode = http.responseStatusCode();
    Serial.printf("[  TEST  ] http %d\n", responseCode);
    TEST_ASSERT_GREATER_OR_EQUAL_MESSAGE(400, responseCode, "not a 4xx response");
    TEST_ASSERT_LESS_THAN_MESSAGE(500, responseCode, "not a 4xx response");

    String body = http.responseBody();
    Serial.println(body);
    http.stop();
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

    RUN_TEST(test_modem_connection);
    RUN_TEST(test_world_wide_wait);

    if (modem.isGprsConnected()) modem.gprsDisconnect();

    UNITY_END();
}

void loop()
{
    digitalWrite(LED_BUILTIN, HIGH);
    delay(250);
    digitalWrite(LED_BUILTIN, LOW);
    delay(250);    
}
