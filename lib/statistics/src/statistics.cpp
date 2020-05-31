#include <statistics.h>

void Statistics::update(float value)
  {
    min = value < min ? value : min;
    max = value > max ? value : max;
  }

