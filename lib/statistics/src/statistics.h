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
  float_vec_t history;
  uint16_t maxHistoryLength;

  Statistics(uint32_t maxLength=32);

  void update(float value);
  uint32_t size();
  float mean();
};
#endif