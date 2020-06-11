#include <unity.h>
#include <statistics.h>

void test_function_statistics_initial(void)
{
    Statistics stats;
    TEST_ASSERT_EQUAL_FLOAT(float(1e12), stats.min);
    TEST_ASSERT_EQUAL_FLOAT(float(-1e12), stats.max);
    TEST_ASSERT_EQUAL_FLOAT(0, stats.mean());
    TEST_ASSERT_EQUAL_UINT32(0, stats.size());
}

void test_function_statistics_single(void)
{
    Statistics stats(8);
    stats.update(2);
    TEST_ASSERT_EQUAL_FLOAT(2., stats.min);
    TEST_ASSERT_EQUAL_FLOAT(2., stats.max);
    TEST_ASSERT_EQUAL_FLOAT(2., stats.mean());
    TEST_ASSERT_EQUAL_UINT32(1, stats.size());
}

void test_function_statistics_simple(void)
{
    Statistics stats(8);
    stats.update(0);
    stats.update(1);
    TEST_ASSERT_EQUAL_FLOAT(0., stats.min);
    TEST_ASSERT_EQUAL_FLOAT(1., stats.max);
    TEST_ASSERT_EQUAL_FLOAT(.5, stats.mean());
    TEST_ASSERT_EQUAL_UINT32(2, stats.size());
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
    TEST_ASSERT_EQUAL_UINT32(4, stats.size());
}

void test_function_statistics_huge(void)
{
    Statistics stats(10000);
    for (int i = 0; i <= 20000; i++)
    {
        stats.update(i % 100);
    }
    TEST_ASSERT_EQUAL_FLOAT(0., stats.min);
    TEST_ASSERT_EQUAL_FLOAT(99., stats.max);
    TEST_ASSERT_EQUAL_FLOAT(49.5, stats.mean());
    TEST_ASSERT_EQUAL_UINT32(10000, stats.size());
}
