#ifndef STATISTICS_H
#define STATISTICS_H

#include <cfloat>

class Statistics
{
public:
  float min;
  float max;
  Statistics();
  void update(float value);
};
#endif