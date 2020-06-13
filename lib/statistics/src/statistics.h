#ifndef STATISTICS_H
#define STATISTICS_H

#include <stdint.h>
#include <vector>
#include <cmath>
#include <stdexcept>

#ifdef ARDUINO
#include <Arduino.h>
#endif

using namespace std;

typedef std::vector<float> float_vec_t;
typedef std::vector<int8_t> int8_vec_t;
typedef std::pair<float, float> Point;

class Statistics
{
public:
  float min;
  float max;
  // float_vec_t history;
  uint32_t maxHistoryLength;
  uint32_t entries = 0;

  vector<Point> history;

  Statistics(uint32_t maxLength = 32);

  bool update(float value);
  uint32_t size();
  float mean();
  bool compact(float epsilon = .2);

  float perpendicularDistance(const Point &pt, const Point &lineStart, const Point &lineEnd);
  void ramerDouglasPeucker(const vector<Point> &pointList, float epsilon, vector<Point> &out);
};
#endif