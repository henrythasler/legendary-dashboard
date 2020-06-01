#include <statistics.h>

Statistics::Statistics()
{
  min = 1e12;
  max = -1e12;
}

void Statistics::update(float value)
{
  min = value < min ? value : min;
  max = value > max ? value : max;
}
