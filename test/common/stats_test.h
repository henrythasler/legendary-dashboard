#include <unity.h>
#include <statistics.h>
#include <stdio.h>

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

    stats.RamerDouglasPeucker(examplePoint, 1.0, pointListOut);

    TEST_ASSERT_EQUAL_INT16_MESSAGE(pointListOut.size(), 2, "pointListOut.size()");

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

    stats.RamerDouglasPeucker(examplePoint, 1.0, pointListOut);

    TEST_ASSERT_EQUAL_INT16_MESSAGE(pointListOut.size(), 3, "pointListOut.size()");

    TEST_ASSERT_EQUAL_FLOAT(5., pointListOut.at(0).first);
    TEST_ASSERT_EQUAL_FLOAT(0., pointListOut.at(0).second);

    TEST_ASSERT_EQUAL_FLOAT(3., pointListOut.at(1).first);
    TEST_ASSERT_EQUAL_FLOAT(0, pointListOut.at(1).second);

    TEST_ASSERT_EQUAL_FLOAT(3., pointListOut.at(2).first);
    TEST_ASSERT_EQUAL_FLOAT(2, pointListOut.at(2).second);
}

// from https://github.com/LukaszWiktor/series-reducer
void test_function_statistics_rdp_math1(void)
{
    Statistics stats;
    char message[64];
    vector<Point>::const_iterator i;

    float_vec_t exampleFlat;

    vector<Point> examplePoint;
    vector<Point> pointListOut;

    for (float x = 0; x < 4; x += 0.05)
    {
        examplePoint.push_back(Point(x, cos(x * x - 1)));
        exampleFlat.push_back(cos(x * x - 1));
    }

    stats.RamerDouglasPeucker(examplePoint, .01, pointListOut);

    int sizePoint = sizeof(examplePoint) + sizeof(Point) * examplePoint.capacity();
    int sizePointPack = sizeof(pointListOut) + sizeof(Point) * pointListOut.capacity();
    int sizeFlat = sizeof(exampleFlat) + sizeof(float) * exampleFlat.capacity();    

    snprintf(message, 64, "sizePoint=%u vs. sizeFlat=%u vs. pointAfterRDP=%u", sizePoint, sizeFlat, sizePointPack);
    TEST_MESSAGE(message);

    // for (i = pointListOut.begin(); i != pointListOut.end(); ++i)
    // {
    //     snprintf(message, 64, "%.2f, %.2f", float(Point(*i).first), float(Point(*i).second));
    //     TEST_MESSAGE(message);
    // }    

    TEST_ASSERT_EQUAL_INT16_MESSAGE(pointListOut.size(), 39, "pointListOut.size()");
}

// void test_function_statistics_rdp_synth(void)
// {
//     vector<Point>::const_iterator i;
//     char message[64];

//     float_vec_t example;
//     vector<Point> examplePoint;

//     vector<Point> pointListOut;

//     Statistics stats(16);
//     // stats.update(0);
//     // stats.update(0.1);
//     // stats.update(-0.1);
//     // stats.update(5);
//     // stats.update(6);
//     // stats.update(7);
//     // stats.update(8.1);
//     // stats.update(9);
//     // stats.update(9);
//     // stats.update(9);

//     for(int j=0; j<100;j++) {
//         example.push_back(float(j));
//         examplePoint.push_back(Point(float(j),float(j%10)));
//     }
//     stats.RamerDouglasPeucker(examplePoint, 1.0, pointListOut);

//     stats.history.push_back(Point(0.0, 0.0));
//     stats.history.push_back(Point(1.0, 0.1));
//     stats.history.push_back(Point(2.0, -0.1));
//     stats.history.push_back(Point(3.0, 5.0));
//     stats.history.push_back(Point(4.0, 6.0));
//     stats.history.push_back(Point(5.0, 7.0));
//     stats.history.push_back(Point(6.0, 8.1));
//     stats.history.push_back(Point(7.0, 9.0));
//     stats.history.push_back(Point(8.0, 9.0));
//     stats.history.push_back(Point(9.0, 9.0));

//     int sizePoint = sizeof(examplePoint) + sizeof(Point) * examplePoint.capacity();
//     int sizePointPack = sizeof(pointListOut) + sizeof(Point) * pointListOut.capacity();
//     int sizeFlat = sizeof(example) + sizeof(float) * example.capacity();

//     snprintf(message, 64, "sizePoint=%u vs. sizeFlat=%u pointListOut=%u", sizePoint, sizeFlat, sizePointPack);
//     TEST_MESSAGE(message);

//     stats.RamerDouglasPeucker(stats.history, 1.0, pointListOut);
//     TEST_ASSERT_EQUAL_INT16_MESSAGE(pointListOut.size(), 5, "pointListOut.size()");

//     for (i = pointListOut.begin(); i != pointListOut.end(); ++i)
//     {
//         snprintf(message, 64, "%.2f, %.2f", float(Point(*i).first), float(Point(*i).second));
//         TEST_MESSAGE(message);
//     }

//     TEST_ASSERT_EQUAL_FLOAT(0., pointListOut.at(0).first);
//     TEST_ASSERT_EQUAL_FLOAT(0., pointListOut.at(0).second);

//     TEST_ASSERT_EQUAL_FLOAT(2., pointListOut.at(1).first);
//     TEST_ASSERT_EQUAL_FLOAT(-0.1, pointListOut.at(1).second);

//     TEST_ASSERT_EQUAL_FLOAT(3., pointListOut.at(2).first);
//     TEST_ASSERT_EQUAL_FLOAT(5., pointListOut.at(2).second);

//     TEST_ASSERT_EQUAL_FLOAT(7., pointListOut.at(3).first);
//     TEST_ASSERT_EQUAL_FLOAT(9., pointListOut.at(3).second);

//     TEST_ASSERT_EQUAL_FLOAT(9., pointListOut.at(4).first);
//     TEST_ASSERT_EQUAL_FLOAT(9., pointListOut.at(4).second);
// }
