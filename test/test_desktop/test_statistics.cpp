#include <statistics.h>
#include <unity.h>

// void setUp(void) {
//     Statistics stats;
// }

// void tearDown(void) {
// // clean stuff up here
// }

void test_function_statistics_initial(void)
{
    Statistics stats;
    TEST_ASSERT_EQUAL_FLOAT(float(1e12), stats.min);
    TEST_ASSERT_EQUAL_FLOAT(float(-1e12), stats.max);
    TEST_ASSERT_EQUAL_FLOAT(0, stats.mean());
}

void test_function_statistics_single(void)
{
    Statistics stats(8);
    stats.update(2);
    TEST_ASSERT_EQUAL_FLOAT(2., stats.min);
    TEST_ASSERT_EQUAL_FLOAT(2., stats.max);
    TEST_ASSERT_EQUAL_FLOAT(2., stats.mean());
}

void test_function_statistics_simple(void)
{
    Statistics stats(8);
    stats.update(0);
    stats.update(1);
    TEST_ASSERT_EQUAL_FLOAT(0., stats.min);
    TEST_ASSERT_EQUAL_FLOAT(1., stats.max);
    TEST_ASSERT_EQUAL_FLOAT(.5, stats.mean());
}

void test_function_statistics_limiter(void)
{
    Statistics stats(4);
    stats.update(0);
    stats.update(1);
    stats.update(2);
    stats.update(3);
    stats.update(4);
    stats.update(5);
    TEST_ASSERT_EQUAL_FLOAT(0., stats.min);
    TEST_ASSERT_EQUAL_FLOAT(5., stats.max);
    TEST_ASSERT_EQUAL_FLOAT(3.5, stats.mean());
}

void test_function_statistics_huge(void)
{
    Statistics stats(32000);
    for (int i = 0; i <= 64000; i++)
    {
        stats.update(i % 100);
    }
    TEST_ASSERT_EQUAL_FLOAT(0., stats.min);
    TEST_ASSERT_EQUAL_FLOAT(99., stats.max);
    TEST_ASSERT_EQUAL_FLOAT(49.5, stats.mean());
}

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