#include <unity.h>
#include <statistics.h>

#ifndef ARDUINO
#include <stdio.h>
#endif

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
    for (int i = 0; i <= 9000; i++)
    {
        stats.update(i % 100);
#ifdef ARDUINO
        if (!(i % 100))
            Serial.printf("%u Heap: %uKiB free\n", i, ESP.getFreeHeap() / 1024);
#endif
    }
    TEST_ASSERT_EQUAL_FLOAT(0., stats.min);
    TEST_ASSERT_EQUAL_FLOAT(99., stats.max);
    TEST_ASSERT_EQUAL_FLOAT(49.5, stats.mean());
    TEST_ASSERT_EQUAL_UINT32(10000, stats.size());

#ifndef ARDUINO
    char message[64];
    int size = sizeof(stats.history) + sizeof(Point) * stats.history.capacity();
    snprintf(message, 64, "size=%u", size);
    TEST_MESSAGE(message);
#endif
}

void test_function_statistics_rdp_synth1(void)
{
    Statistics stats;
    vector<Point>::const_iterator i;

    vector<Point> examplePoint;
    vector<Point> pointListOut;

    examplePoint.push_back(Point(1., 1.));
    examplePoint.push_back(Point(2., 2.));
    examplePoint.push_back(Point(3., 3.));
    examplePoint.push_back(Point(4., 4.));

    stats.ramerDouglasPeucker(examplePoint, 1.0, pointListOut);

    TEST_ASSERT_EQUAL_INT16_MESSAGE(2, pointListOut.size(), "pointListOut.size()");

    TEST_ASSERT_EQUAL_FLOAT(1., pointListOut.at(0).first);
    TEST_ASSERT_EQUAL_FLOAT(1., pointListOut.at(0).second);

    TEST_ASSERT_EQUAL_FLOAT(4., pointListOut.at(1).first);
    TEST_ASSERT_EQUAL_FLOAT(4, pointListOut.at(1).second);
}

void test_function_statistics_rdp_synth2(void)
{
    Statistics stats;
    vector<Point>::const_iterator i;

    vector<Point> examplePoint;
    vector<Point> pointListOut;
    examplePoint.push_back(Point(5., 0.));
    examplePoint.push_back(Point(4., 0.));
    examplePoint.push_back(Point(3., 0.));
    examplePoint.push_back(Point(3., 1.));
    examplePoint.push_back(Point(3., 2.));

    stats.ramerDouglasPeucker(examplePoint, 1.0, pointListOut);

    TEST_ASSERT_EQUAL_INT16_MESSAGE(3, pointListOut.size(), "pointListOut.size()");

    TEST_ASSERT_EQUAL_FLOAT(5., pointListOut.at(0).first);
    TEST_ASSERT_EQUAL_FLOAT(0., pointListOut.at(0).second);

    TEST_ASSERT_EQUAL_FLOAT(3., pointListOut.at(1).first);
    TEST_ASSERT_EQUAL_FLOAT(0, pointListOut.at(1).second);

    TEST_ASSERT_EQUAL_FLOAT(3., pointListOut.at(2).first);
    TEST_ASSERT_EQUAL_FLOAT(2, pointListOut.at(2).second);
}

// from https://github.com/LukaszWiktor/series-reducer
void test_function_statistics_rdp_math(void)
{
    Statistics stats;
    vector<Point>::const_iterator i;

    float_vec_t exampleFlat;

    vector<Point> examplePoint;
    vector<Point> pointListOut;

    for (float x = 0; x <= 4; x += 0.05)
    {
        examplePoint.push_back(Point(x, cos(x * x - 1)));
        exampleFlat.push_back(cos(x * x - 1));
    }

    stats.ramerDouglasPeucker(examplePoint, .01, pointListOut);
    pointListOut.shrink_to_fit();

#ifndef ARDUINO
    char message[64];
    int sizePoint = sizeof(examplePoint) + sizeof(Point) * examplePoint.capacity();
    int sizePointPack = sizeof(pointListOut) + sizeof(Point) * pointListOut.capacity();
    int sizeFlat = sizeof(exampleFlat) + sizeof(float) * exampleFlat.capacity();

    snprintf(message, 64, "sizePoint=%u vs. sizeFlat=%u vs. pointAfterRDP=%u", sizePoint, sizeFlat, sizePointPack);
    TEST_MESSAGE(message);
    // for (vector<Point>::const_iterator i = pointListOut.begin(); i != pointListOut.end(); ++i)
    // {
    //     snprintf(message, 64, "%.2f, %.2f", float(Point(*i).first), float(Point(*i).second));
    //     TEST_MESSAGE(message);
    // }
#endif

    TEST_ASSERT_EQUAL_INT16_MESSAGE(39, pointListOut.size(), "pointListOut.size()");
}

void test_function_statistics_compact_math(void)
{
    Statistics stats(256);
    float_vec_t exampleFlat;

    for (int x = 0; x <= 80; x++)
    {
        stats.update(cos((float(x) / 20.) * (float(x) / 20.) - 1) * 20);
    }

#ifndef ARDUINO
    int before = sizeof(stats.history) + sizeof(Point) * stats.history.capacity();
#endif
    stats.compact(0.2);
#ifndef ARDUINO
    int after = sizeof(stats.history) + sizeof(Point) * stats.history.capacity();
#endif

#ifndef ARDUINO
    char message[64];
    // for (vector<Point>::const_iterator i = stats.history.begin(); i != stats.history.end(); ++i)
    // {
    //     snprintf(message, 64, "%.2f, %.2f", float(Point(*i).first), float(Point(*i).second));
    //     TEST_MESSAGE(message);
    // }
    snprintf(message, 64, "before=%u vs. after=%u", before, after);
    TEST_MESSAGE(message);
#endif

    TEST_ASSERT_EQUAL_INT16_MESSAGE(39, stats.history.size(), "stats.history.size()");
}

void test_function_statistics_compact_throw1(void)
{
    Statistics stats;

    stats.update(0);

#ifndef ARDUINO
    int before = sizeof(stats.history) + sizeof(Point) * stats.history.capacity();
#endif

    bool res = stats.compact(0.2);
    TEST_ASSERT_FALSE(res);

#ifndef ARDUINO
    int after = sizeof(stats.history) + sizeof(Point) * stats.history.capacity();
#endif

#ifndef ARDUINO
    char message[64];
    snprintf(message, 64, "before=%u vs. after=%u", before, after);
    TEST_MESSAGE(message);
#endif

    TEST_ASSERT_EQUAL_INT16_MESSAGE(1, stats.history.size(), "stats.history.size()");
}

void process(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_function_statistics_initial);
    RUN_TEST(test_function_statistics_single);
    RUN_TEST(test_function_statistics_simple);
    RUN_TEST(test_function_statistics_limiter);
    RUN_TEST(test_function_statistics_huge);
    RUN_TEST(test_function_statistics_rdp_synth1);
    RUN_TEST(test_function_statistics_rdp_synth2);
    RUN_TEST(test_function_statistics_rdp_math);
    RUN_TEST(test_function_statistics_compact_math);
    RUN_TEST(test_function_statistics_compact_throw1);
    UNITY_END();
}

#ifdef ARDUINO
#include <Arduino.h>
#define LED_BUILTIN (13) // LED is connected to IO13

void setup()
{
    // NOTE!!! Wait for >2 secs
    // if board doesn't support software reset via Serial.DTR/RTS
    delay(2000);

    process();
}

void loop()
{
    digitalWrite(LED_BUILTIN, HIGH);
    delay(100);
    digitalWrite(LED_BUILTIN, LOW);
    delay(500);
}

#else

int main(int argc, char **argv)
{
    process();
    return 0;
}
#endif