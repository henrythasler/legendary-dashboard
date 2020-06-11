#include <statistics.h>

Statistics::Statistics(uint32_t maxLength)
{
  min = 1e12;
  max = -1e12;
  maxHistoryLength = maxLength;
}

void Statistics::update(float value)
{
  min = value < min ? value : min;
  max = value > max ? value : max;
  if (history.size() >= maxHistoryLength)
  {
    history.erase(history.begin());
  }
  history.push_back(value);
}

float Statistics::mean()
{
  vector<float>::const_iterator i;
  float mean = 0;

  if (history.size())
  {
    mean = history[0];
    for (i = history.begin() + 1; i != history.end(); ++i)
    {
      mean += float(*i);
    }
    mean = mean / history.size();
  }
  return mean;
}

uint32_t Statistics::size()
{
  return history.size();
}