#include <statistics.h>

Statistics::Statistics(uint32_t maxLength)
{
  min = 1e12;
  max = -1e12;
  maxHistoryLength = maxLength;
}

bool Statistics::update(float value)
{
  min = value < min ? value : min;
  max = value > max ? value : max;

  try
  {
    if (history.size() >= maxHistoryLength)
      history.erase(history.begin());
    history.push_back(Point(id, value));
  }
  catch (const exception &e)
  {
#ifdef ARDUINO
    Serial.printf("Error in Statistics::update(): %s\n", e.what());
#else
    printf("Error in Statistics::update(): %s\n", e.what());
#endif
    return false;
  }

  id++;
  return true;
}

float Statistics::mean()
{
  float mean = 0;

  if (history.size())
  {
    mean = history[0].second;
    for (PointIterator i = history.begin() + 1; i != history.end(); ++i)
    {
      mean += float(Point(*i).second);
    }
    mean = mean / history.size();
  }
  return mean;
}

uint32_t Statistics::size()
{
  return history.size();
}

// from: https://rosettacode.org/wiki/Ramer-Douglas-Peucker_line_simplification#C.2B.2B
float Statistics::perpendicularDistance(const Point &pt, const Point &lineStart, const Point &lineEnd)
{
  float dx = float(lineEnd.first) - float(lineStart.first);
  float dy = lineEnd.second - lineStart.second;

  //Normalise
  float mag = pow(pow(dx, 2.0) + pow(dy, 2.0), 0.5);
  if (mag > 0.0)
  {
    dx /= mag;
    dy /= mag;
  }

  float pvx = float(pt.first) - float(lineStart.first);
  float pvy = pt.second - lineStart.second;

  //Get dot product (project pv onto normalized direction)
  float pvdot = dx * pvx + dy * pvy;

  //Scale line direction vector
  float dsx = pvdot * dx;
  float dsy = pvdot * dy;

  //Subtract this from pv
  float ax = pvx - dsx;
  float ay = pvy - dsy;

  return pow(pow(ax, 2.0) + pow(ay, 2.0), 0.5);
}

void Statistics::ramerDouglasPeucker(const vector<Point> &pointList, float epsilon, vector<Point> &out)
{
  if (pointList.size() < 2)
    throw invalid_argument("Not enough points to simplify");

  // Find the point with the maximum distance from line between start and end
  float dmax = 0.0;
  size_t index = 0;
  size_t end = pointList.size() - 1;
  for (size_t i = 1; i < end; i++)
  {
    float d = perpendicularDistance(pointList[i], pointList[0], pointList[end]);
    if (d > dmax)
    {
      index = i;
      dmax = d;
    }
  }

  // If max distance is greater than epsilon, recursively simplify
  if (dmax > epsilon)
  {
    // Recursive call
    vector<Point> recResults1;
    vector<Point> recResults2;
    vector<Point> firstLine(pointList.begin(), pointList.begin() + index + 1);
    vector<Point> lastLine(pointList.begin() + index, pointList.end());
    ramerDouglasPeucker(firstLine, epsilon, recResults1);
    ramerDouglasPeucker(lastLine, epsilon, recResults2);

    // Build the result list
    out.assign(recResults1.begin(), recResults1.end() - 1);
    out.insert(out.end(), recResults2.begin(), recResults2.end());
    if (out.size() < 2)
      throw runtime_error("Problem assembling output");
  }
  else
  {
    //Just return start and end points
    out.clear();
    out.push_back(pointList[0]);
    out.push_back(pointList[end]);
  }
}

/**
* This will apply the Ramer-Douglas-Peucker algorithm to the dataset stored in the history-vector.
* @param epsilon Larger values will result in fewer data points
*/
bool Statistics::compact(float epsilon)
{
  vector<Point> pointListOut;
  try
  {
    ramerDouglasPeucker(history, epsilon, pointListOut);
    history.assign(pointListOut.begin(), pointListOut.end());
    history.shrink_to_fit();
  }
  catch (const std::exception &e)
  {
#ifdef ARDUINO
    Serial.printf("Error in Statistics::compact(%f): %s\n", epsilon, e.what());
#else
    printf("Error in Statistics::compact(%f): %s\n", epsilon, e.what());
#endif
    return false;
  }
  return true;
}