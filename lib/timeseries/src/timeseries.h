#ifndef STATISTICS_H
#define STATISTICS_H

#include <stdint.h>
#include <vector>
#include <cmath>
#include <stdexcept>
#include <sys/time.h>
#include <algorithm>

#ifdef ARDUINO
#include <Arduino.h>
#endif

using namespace std;

typedef struct
{
  uint32_t time;
  float value;
} Point;

typedef vector<Point>::const_iterator PointIterator;

#ifndef PI
#define PI 3.14159265358979323846 /* pi */
#endif

class Timeseries
{
public:
  float min;
  float max;
  uint32_t maxHistoryLength;
  uint32_t id = 0;

  vector<Point> data;
  vector<float> kernel;

  Timeseries(uint32_t maxLength = 32);

  bool push(uint32_t timestamp, float value);
  void updateStats(void);
  uint32_t size();
  uint32_t capacity();
  float mean();
  int32_t compact(float epsilon = .2);
  int32_t trim(uint32_t currentTimeSeconds, uint32_t maxAgeSeconds = 604800);

  float perpendicularDistance(const Point &pt, const Point &lineStart, const Point &lineEnd);
  void ramerDouglasPeucker(const vector<Point> &pointList, float epsilon, vector<Point> &out);

  void movingAverage(int32_t samples = 5);
};
#endif