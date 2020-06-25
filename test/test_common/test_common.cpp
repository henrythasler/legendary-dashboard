#include <unity.h>
#include <timeseries.h>

#ifndef ARDUINO
#include <stdio.h>
#include <chrono>
#endif

void test_function_statistics_initial(void)
{
  Timeseries series;
  TEST_ASSERT_EQUAL_FLOAT(float(1e12), series.min);
  TEST_ASSERT_EQUAL_FLOAT(float(-1e12), series.max);
  TEST_ASSERT_EQUAL_FLOAT(0, series.mean());
  TEST_ASSERT_EQUAL_UINT32(0, series.size());
}

void test_function_statistics_single(void)
{
  Timeseries series(8);
  series.push(0, 2);
  TEST_ASSERT_EQUAL_FLOAT(2., series.min);
  TEST_ASSERT_EQUAL_FLOAT(2., series.max);
  TEST_ASSERT_EQUAL_FLOAT(2., series.mean());
  TEST_ASSERT_EQUAL_UINT32(1, series.size());
}

void test_function_statistics_simple(void)
{
  Timeseries series(8);
  series.push(0, 0);
  series.push(1, 1);
  TEST_ASSERT_EQUAL_FLOAT(0., series.min);
  TEST_ASSERT_EQUAL_FLOAT(1., series.max);
  TEST_ASSERT_EQUAL_FLOAT(.5, series.mean());
  TEST_ASSERT_EQUAL_UINT32(2, series.size());
}

void test_function_statistics_limiter(void)
{
  Timeseries series(4);
  series.push(0, 0);
  series.push(1, 1);
  series.push(2, 2);
  series.push(3, 3);
  series.push(4, 4);
  series.push(5, 5);

  TEST_ASSERT_EQUAL_FLOAT_MESSAGE(2., series.min, "series.min");
  TEST_ASSERT_EQUAL_FLOAT_MESSAGE(5., series.max, "series.max");
  TEST_ASSERT_EQUAL_FLOAT_MESSAGE(3.5, series.mean(), "series.mean()");
  TEST_ASSERT_EQUAL_UINT32(4, series.size());

  TEST_ASSERT_EQUAL_INT32(2, series.data.at(0).first);
  TEST_ASSERT_EQUAL_FLOAT(2., series.data.at(0).second);
  TEST_ASSERT_EQUAL_INT32(5, series.data.at(3).first);
  TEST_ASSERT_EQUAL_FLOAT(5., series.data.at(3).second);
}

void test_function_statistics_huge(void)
{
  int maxEntries = 20000;
#ifdef ARDUINO
  maxEntries = 5000;
#endif

  Timeseries series(maxEntries);
  for (int i = 0; i < 50000; i++)
  {
    series.push(i, i % 100);
  }
  TEST_ASSERT_EQUAL_FLOAT(0., series.min);
  TEST_ASSERT_EQUAL_FLOAT(99., series.max);
  TEST_ASSERT_EQUAL_FLOAT(49.5, series.mean());
  TEST_ASSERT_EQUAL_UINT32(maxEntries, series.size());

#ifndef ARDUINO
  char message[64];
  int size = sizeof(series.data) + sizeof(Point) * series.data.capacity();
  snprintf(message, 64, "size=%u", size);
  TEST_MESSAGE(message);
#endif
}

void test_function_statistics_rdp_synth1(void)
{
  Timeseries series;
  vector<Point>::const_iterator i;

  vector<Point> examplePoint;
  vector<Point> pointListOut;

  examplePoint.push_back(Point(1., 1.));
  examplePoint.push_back(Point(2., 2.));
  examplePoint.push_back(Point(3., 3.));
  examplePoint.push_back(Point(4., 4.));

  series.ramerDouglasPeucker(examplePoint, 1.0, pointListOut);

  TEST_ASSERT_EQUAL_INT32_MESSAGE(2, pointListOut.size(), "pointListOut.size()");

  TEST_ASSERT_EQUAL_INT32(1, pointListOut.at(0).first);
  TEST_ASSERT_EQUAL_FLOAT(1., pointListOut.at(0).second);

  TEST_ASSERT_EQUAL_INT32(4, pointListOut.at(1).first);
  TEST_ASSERT_EQUAL_FLOAT(4, pointListOut.at(1).second);
}

void test_function_statistics_rdp_synth2(void)
{
  Timeseries series;
  vector<Point>::const_iterator i;

  vector<Point> examplePoint;
  vector<Point> pointListOut;
  examplePoint.push_back(Point(5, 0.));
  examplePoint.push_back(Point(4, 0.));
  examplePoint.push_back(Point(3, 0.));
  examplePoint.push_back(Point(3, 1.));
  examplePoint.push_back(Point(3, 2.));

  series.ramerDouglasPeucker(examplePoint, 1.0, pointListOut);

  TEST_ASSERT_EQUAL_INT32_MESSAGE(3, pointListOut.size(), "pointListOut.size()");

  TEST_ASSERT_EQUAL_INT32(5, pointListOut.at(0).first);
  TEST_ASSERT_EQUAL_FLOAT(0., pointListOut.at(0).second);

  TEST_ASSERT_EQUAL_INT32(3, pointListOut.at(1).first);
  TEST_ASSERT_EQUAL_FLOAT(0, pointListOut.at(1).second);

  TEST_ASSERT_EQUAL_INT32(3, pointListOut.at(2).first);
  TEST_ASSERT_EQUAL_FLOAT(2, pointListOut.at(2).second);
}

// from https://github.com/LukaszWiktor/series-reducer
void test_function_statistics_rdp_math(void)
{
  Timeseries series;
  vector<Point>::const_iterator i;

  float_vec_t exampleFlat;

  vector<Point> examplePoint;
  vector<Point> pointListOut;

  for (int x = 0; x <= 80; x++)
  {
    examplePoint.push_back(Point(x, cos((float(x) / 20.) * (float(x) / 20.) - 1) * 20));
    exampleFlat.push_back(cos((float(x) / 20.) * (float(x) / 20.) - 1) * 20);
  }

  series.ramerDouglasPeucker(examplePoint, .2, pointListOut);
  pointListOut.shrink_to_fit();

#ifndef ARDUINO
  char message[64];
  int sizePoint = sizeof(examplePoint) + sizeof(Point) * examplePoint.capacity();
  int sizePointPack = sizeof(pointListOut) + sizeof(Point) * pointListOut.capacity();
  int sizeFlat = sizeof(exampleFlat) + sizeof(float) * exampleFlat.capacity();

  snprintf(message, 64, "sizePoint=%u vs. sizeFlat=%u vs. pointAfterRDP=%u", sizePoint, sizeFlat, sizePointPack);
  TEST_MESSAGE(message);
#endif

  TEST_ASSERT_EQUAL_INT32_MESSAGE(39, pointListOut.size(), "pointListOut.size()");
}

void test_function_statistics_compact_math(void)
{
  Timeseries series(256);
  float_vec_t exampleFlat;

  for (int x = 0; x <= 80; x++)
  {
    series.push(x, cos((float(x) / 20.) * (float(x) / 20.) - 1) * 20);
  }

#ifndef ARDUINO
  int before = sizeof(series.data) + sizeof(Point) * series.data.capacity();
#endif

  int32_t removed = series.compact(0.2);
  TEST_ASSERT_EQUAL_INT32(42, removed);

#ifndef ARDUINO
  int after = sizeof(series.data) + sizeof(Point) * series.data.capacity();
  char message[64];
  snprintf(message, 64, "before=%u vs. after=%u", before, after);
  TEST_MESSAGE(message);
#endif

  TEST_ASSERT_EQUAL_INT32_MESSAGE(39, series.data.size(), "series.data.size()");
}

void test_function_statistics_compact_throw(void)
{
  Timeseries series;
  series.push(0, 0);
  int32_t removed = series.compact();
  TEST_ASSERT_EQUAL_INT32(-1, removed);
  TEST_ASSERT_EQUAL_INT32_MESSAGE(1, series.data.size(), "series.data.size()");
}

void test_function_statistics_compact_huge(void)
{
  try
  {
    Timeseries series(5000);
    for (uint32_t x = 1; x <= 100000; x++)
    {
      series.push(x - 1, 1);
      if (!(x % 100))
        series.compact();
    }
    TEST_ASSERT_EQUAL_INT32_MESSAGE(2, series.data.size(), "series.data.size()");

    TEST_ASSERT_EQUAL_INT32(0, series.data.front().first);
    TEST_ASSERT_EQUAL_FLOAT(1., series.data.front().second);
    TEST_ASSERT_EQUAL_INT32(99999, series.data.back().first);
    TEST_ASSERT_EQUAL_FLOAT(1., series.data.back().second);
  }
  catch (const std::exception &e)
  {
#ifdef ARDUINO
    Serial.printf("Error in Statistics::update(): %s\n", e.what());
#else
    printf("Error in Statistics::update(): %s\n", e.what());
#endif
  }
}

void test_function_statistics_trim_simple(void)
{
  Timeseries series(10);
  series.push(0, 0);
  series.push(1, 1);
  series.push(2, 2);
  series.push(3, 3);
  series.push(4, 4);
  series.push(5, 5);

  TEST_ASSERT_EQUAL_UINT32(6, series.size());
  uint32_t removed = series.trim(6, 4);
  TEST_ASSERT_EQUAL_UINT32_MESSAGE(2, removed, "removed");
  TEST_ASSERT_EQUAL_UINT32_MESSAGE(4, series.size(), "size()");
  TEST_ASSERT_EQUAL_UINT32_MESSAGE(8, series.capacity(), "capacity()");
  TEST_ASSERT_EQUAL_INT32(2, series.data.at(0).first);
  TEST_ASSERT_EQUAL_FLOAT(2., series.data.at(0).second);
  TEST_ASSERT_EQUAL_INT32(5, series.data.at(3).first);
  TEST_ASSERT_EQUAL_FLOAT(5., series.data.at(3).second);
}

void test_function_statistics_trim_none(void)
{
  Timeseries series(10);
  series.push(0, 0);
  series.push(1, 1);
  series.push(2, 2);
  series.push(3, 3);
  series.push(4, 4);
  series.push(5, 5);

  TEST_ASSERT_EQUAL_UINT32(6, series.size());
  TEST_ASSERT_EQUAL_UINT32_MESSAGE(8, series.capacity(), "capacity() before");
  uint32_t removed = series.trim(6, 10);
  TEST_ASSERT_EQUAL_UINT32_MESSAGE(0, removed, "removed");
  TEST_ASSERT_EQUAL_UINT32_MESSAGE(6, series.size(), "size()");
  TEST_ASSERT_EQUAL_UINT32_MESSAGE(8, series.capacity(), "capacity() after trim()");
  TEST_ASSERT_EQUAL_INT32(0, series.data.at(0).first);
  TEST_ASSERT_EQUAL_FLOAT(0., series.data.at(0).second);
  TEST_ASSERT_EQUAL_INT32(5, series.data.at(5).first);
  TEST_ASSERT_EQUAL_FLOAT(5., series.data.at(5).second);
}

void test_function_statistics_trim_compact_simple(void)
{
  Timeseries series(10);
  series.push(0, 0);
  series.push(1, 1);
  series.push(2, 2);
  series.push(3, 3);
  series.push(4, 4);
  series.push(5, 5);

  TEST_ASSERT_EQUAL_UINT32(6, series.size());
  uint32_t removed = series.trim(6, 4);
  TEST_ASSERT_EQUAL_UINT32_MESSAGE(2, removed, "removed");
  bool compactResult = series.compact();
  TEST_ASSERT_EQUAL_UINT32_MESSAGE(2, series.size(), "size()");
  TEST_ASSERT_EQUAL_UINT32_MESSAGE(2, series.capacity(), "capacity()");
  TEST_ASSERT_EQUAL_INT32(2, series.data.at(0).first);
  TEST_ASSERT_EQUAL_FLOAT(2., series.data.at(0).second);
  TEST_ASSERT_EQUAL_INT32(5, series.data.at(1).first);
  TEST_ASSERT_EQUAL_FLOAT(5., series.data.at(1).second);
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
  RUN_TEST(test_function_statistics_compact_throw);
  RUN_TEST(test_function_statistics_compact_huge);
  RUN_TEST(test_function_statistics_trim_simple);
  RUN_TEST(test_function_statistics_trim_none);
  RUN_TEST(test_function_statistics_trim_compact_simple);
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
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(115200);
  Serial.printf("Heap: %u KiB free\n", ESP.getFreeHeap() / 1024);
  process();
}

void loop()
{
  digitalWrite(LED_BUILTIN, HIGH);
  delay(250);
  digitalWrite(LED_BUILTIN, LOW);
  delay(250);
}

#else

int main(int argc, char **argv)
{
  auto start = std::chrono::high_resolution_clock::now();
  process();
  auto stop = std::chrono::high_resolution_clock::now();
  auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start); 
  printf("Elapsed time: %lums\n", duration.count()/1000);
  return 0;
}
#endif