#include <statistics.h>
#include <unity.h>
#include "../common/stats_test.h"

// void setUp(void) {
//     Statistics stats;
// }

// void tearDown(void) {
// // clean stuff up here
// }

int main(int argc, char **argv)
{
    UNITY_BEGIN();
    RUN_TEST(test_function_statistics_initial);
    RUN_TEST(test_function_statistics_single);
    RUN_TEST(test_function_statistics_simple);
    RUN_TEST(test_function_statistics_limiter);
    RUN_TEST(test_function_statistics_huge);
    UNITY_END();

    return 0;
}