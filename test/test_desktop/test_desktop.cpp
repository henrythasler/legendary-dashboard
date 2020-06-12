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
    RUN_TEST(test_function_statistics_rdp_synth1);
    RUN_TEST(test_function_statistics_rdp_synth2);
    RUN_TEST(test_function_statistics_rdp_math1);
    UNITY_END();

    return 0;
}