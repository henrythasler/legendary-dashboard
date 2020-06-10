#ifndef STATISTICS_H
#define STATISTICS_H

#include <stdint.h>
#include <vector>

using namespace std;

typedef std::vector<float> float_vec_t;
typedef std::vector<int8_t> int8_vec_t;

class Statistics
{
public:
  float min;
  float max;
  int8_vec_t history;   // smaller type to save memory
  uint16_t maxHistoryLength;

  Statistics(uint16_t maxLength=32);

  void update(float value);
  float mean();
};
#endif