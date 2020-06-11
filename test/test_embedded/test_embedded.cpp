#include <Arduino.h>
#include <unity.h>
#include "../common/stats_test.h"

#define LED_BUILTIN (13) // LED is connected to IO13

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

void setup()
{
    UNITY_BEGIN();
    // test statistics lib
    RUN_TEST(test_function_statistics_initial);
    RUN_TEST(test_function_statistics_single);
    RUN_TEST(test_function_statistics_simple);
    RUN_TEST(test_function_statistics_limiter);
    RUN_TEST(test_function_statistics_huge);

    // prepare for I/O test
    TEST_ASSERT_EQUAL(LED_BUILTIN, 13);
    pinMode(LED_BUILTIN, OUTPUT);
}

uint8_t i = 0;
uint8_t max_blinks = 2;

void loop()
{
    if (i < max_blinks)
    {
        RUN_TEST(test_led_state_high);
        delay(100);
        RUN_TEST(test_led_state_low);
        delay(100);
        i++;
    }
    else if (i == max_blinks)
    {
        UNITY_END(); // stop unit testing
    }
}