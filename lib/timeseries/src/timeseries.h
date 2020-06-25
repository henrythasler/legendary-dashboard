#ifndef STATISTICS_H
#define STATISTICS_H

#include <stdint.h>
#include <vector>
#include <cmath>
#include <stdexcept>
#include <sys/time.h>

#ifdef ARDUINO
#include <Arduino.h>
#endif

using namespace std;

typedef std::vector<float> float_vec_t;
typedef std::vector<int8_t> int8_vec_t;
typedef std::pair<uint32_t, float> Point;
typedef vector<Point>::const_iterator PointIterator;

class Timeseries
{
public:
  float min;
  float max;
  uint32_t maxHistoryLength;
  uint32_t id = 0;

  vector<Point> data;

  Timeseries(uint32_t maxLength = 32);

  bool push(uint32_t timestamp, float value);
  uint32_t size();
  uint32_t capacity();
  float mean();
  int32_t compact(float epsilon = .2);
  int32_t trim(uint32_t currentTimeSeconds, uint32_t maxAgeSeconds = 604800);

  float perpendicularDistance(const Point &pt, const Point &lineStart, const Point &lineEnd);
  void ramerDouglasPeucker(const vector<Point> &pointList, float epsilon, vector<Point> &out);
};
#endif