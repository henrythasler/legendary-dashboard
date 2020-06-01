#ifndef STATISTICS_H
#define STATISTICS_H

#include <cfloat>

class Statistics
{
public:
  Statistics() {}
  float min = 1e12;
  float max = -1e12;
  void update(float value);
};
#endif