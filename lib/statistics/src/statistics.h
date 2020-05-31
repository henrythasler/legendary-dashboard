#ifndef STATISTICS_H
#define STATISTICS_H

#include <cfloat>

class Statistics
{
public:
  Statistics() {}
  float min = FLT_MAX;
  float max = FLT_MIN;
  void update(float value);
};
#endif