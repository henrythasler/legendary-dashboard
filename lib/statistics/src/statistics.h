#ifndef STATISTICS_H
#define STATISTICS_H

#include <cfloat>

class Statistics
{
public:
  Statistics() {}
  float min = float(1e12);
  float max = float(-1e12);
  void update(float value);
};
#endif