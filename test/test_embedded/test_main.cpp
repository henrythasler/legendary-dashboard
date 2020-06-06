#include <Arduino.h>
#include <unity.h>

#define LED_BUILTIN (13) // LED is connected to IO13

#include <statistics.h>
Statistics stats;

// void setUp(void) {
// // set stuff up here
// }

// void tearDown(void) {
// // clean stuff up here
// }

void test_led_builtin_pin_number(void) {
    TEST_ASSERT_EQUAL(LED_BUILTIN, 13);
}

void test_led_state_high(void) {
    digitalWrite(LED_BUILTIN, HIGH);
    TEST_ASSERT_EQUAL(digitalRead(LED_BUILTIN), HIGH);
}

void test_led_state_low(void) {
    digitalWrite(LED_BUILTIN, LOW);
    TEST_ASSERT_EQUAL(digitalRead(LED_BUILTIN), LOW);
}

void test_function_statistics_initial(void) {
    TEST_ASSERT_EQUAL(float(1e12), stats.min);
    TEST_ASSERT_EQUAL(float(-1e12), stats.max);
}

void test_function_statistics_simple(void) {
    stats.update(0);
    stats.update(1);
    TEST_ASSERT_EQUAL(0., stats.min);
    TEST_ASSERT_EQUAL(1., stats.max);
}

void setup() {
    UNITY_BEGIN();
    RUN_TEST(test_led_builtin_pin_number);
    pinMode(LED_BUILTIN, OUTPUT);

    RUN_TEST(test_function_statistics_initial);
    RUN_TEST(test_function_statistics_simple);    
}

uint8_t i = 0;
uint8_t max_blinks = 5;

void loop() {
    if (i < max_blinks)
    {
        RUN_TEST(test_led_state_high);
        delay(500);
        RUN_TEST(test_led_state_low);
        delay(500);
        i++;
    }
    else if (i == max_blinks) {
      UNITY_END(); // stop unit testing
    }
}