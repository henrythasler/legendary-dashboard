#include <statistics.h>
#include <unity.h>

Statistics stats;

// void setUp(void) {
//     Statistics stats;
// }

// void tearDown(void) {
// // clean stuff up here
// }

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

int main(int argc, char **argv) {
    UNITY_BEGIN();
    RUN_TEST(test_function_statistics_initial);
    RUN_TEST(test_function_statistics_simple);
    UNITY_END();

    return 0;
}