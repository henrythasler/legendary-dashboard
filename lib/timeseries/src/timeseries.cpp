#include <timeseries.h>

Timeseries::Timeseries(uint32_t maxLength)
{
  min = 1e12;
  max = -1e12;
  maxHistoryLength = maxLength;
}

bool Timeseries::push(uint32_t timestamp, float value)
{
  bool updateStats = false;
  min = value < min ? value : min;
  max = value > max ? value : max;

  try
  {
    if (data.size() >= maxHistoryLength)
    {
      updateStats = true;
      data.erase(data.begin());
    }
    data.push_back(Point({timestamp, value}));

    if (updateStats)
    {
      min = 1e12;
      max = -1e12;
      float val = 0;
      for (PointIterator i = data.begin(); i != data.end(); ++i)
      {
        val = float(Point(*i).value);
        min = val < min ? val : min;
        max = val > max ? val : max;
      }
    }
    id++;
  }
  catch (const exception &e)
  {
#ifdef ARDUINO
    Serial.printf("[ ERROR ] Statistics::push(): %s\n", e.what());
#else
    printf("Error in Statistics::push(): %s\n", e.what());
#endif
    return false;
  }

  return true;
}

float Timeseries::mean()
{
  float mean = 0;

  if (data.size())
  {
    mean = data[0].value;
    for (PointIterator i = data.begin() + 1; i != data.end(); ++i)
    {
      mean += float(Point(*i).value);
    }
    mean = mean / data.size();
  }
  return mean;
}

uint32_t Timeseries::size()
{
  return data.size();
}

uint32_t Timeseries::capacity()
{
  return data.capacity();
}

// from: https://rosettacode.org/wiki/Ramer-Douglas-Peucker_line_simplification#C.2B.2B
float Timeseries::perpendicularDistance(const Point &pt, const Point &lineStart, const Point &lineEnd)
{
  float dx = float(lineEnd.time) - float(lineStart.time);
  float dy = lineEnd.value - lineStart.value;

  //Normalise
  float mag = pow(pow(dx, 2.0) + pow(dy, 2.0), 0.5);
  if (mag > 0.0)
  {
    dx /= mag;
    dy /= mag;
  }

  float pvx = float(pt.time) - float(lineStart.time);
  float pvy = pt.value - lineStart.value;

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

void Timeseries::ramerDouglasPeucker(const vector<Point> &pointList, float epsilon, vector<Point> &out)
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
* This will apply the Ramer-Douglas-Peucker algorithm to the dataset stored in the data-vector.
* @param epsilon Larger values will result in fewer data points
*/
int32_t Timeseries::compact(float epsilon)
{
  int32_t removedEntries = 0;
  vector<Point> pointListOut;

  if (data.size())
  {
    try
    {
      ramerDouglasPeucker(data, epsilon, pointListOut);
      removedEntries = data.size() - pointListOut.size();
      data.assign(pointListOut.begin(), pointListOut.end());
      data.shrink_to_fit();
    }
    catch (const std::exception &e)
    {
#ifdef ARDUINO
      Serial.printf("[ ERROR ] Timeseries::compact(%f): %s\n", epsilon, e.what());
#else
      printf("Error in Timeseries::compact(%f): %s\n", epsilon, e.what());
#endif
      removedEntries = -1;
    }
  }
  return removedEntries;
}

/**
 * Checks the timestamps and drops all entries that are older that given by maxAgeSeconds  
 */
int32_t Timeseries::trim(uint32_t currentTimeSeconds, uint32_t maxAgeSeconds)
{
  int32_t removedEntries = 0;
  try
  {
    if (data.size())
    {
      PointIterator i = data.begin();
      while (Point(*i).time + maxAgeSeconds < currentTimeSeconds)
      {
        i++;
      }
      removedEntries = i - data.begin();
      data.erase(data.begin(), i);
    }
  }
  catch (const std::exception &e)
  {
#ifdef ARDUINO
    Serial.printf("[ ERROR ] Timeseries::trim(%u, %u): %s\n", currentTimeSeconds, maxAgeSeconds, e.what());
    removedEntries = -1;
#else
    printf("[ ERROR ] Timeseries::trim(%u, %u): %s\n", currentTimeSeconds, maxAgeSeconds, e.what());
#endif
  }

  return removedEntries;
}

bool Timeseries::computeSpline()
{
  int N = (int)data.size() - 1;
  _p1Points.resize(N);
  _p2Points.resize(N);
  if (N == 0)
    return false;

  if (N == 1)
  { 
    return false;
  }

  /*rhs vector*/
  vector<Point> a(N);
  vector<Point> b(N);
  vector<Point> c(N);
  vector<Point> r(N);

  /*left most segment*/
  a[0].time = 0;
  b[0].time = 2;
  c[0].time = 1;
  r[0].time = data[0].time + 2 * data[1].time;

  a[0].value = 0;
  b[0].value = 2;
  c[0].value = 1;
  r[0].value = data[0].value + 2 * data[1].value;

  /*internal segments*/
  for (int i = 1; i < N - 1; i++)
  {
    a[i].time = 1;
    b[i].time = 4;
    c[i].time = 1;
    r[i].time = 4 * data[i].time + 2 * data[i + 1].time;

    a[i].value = 1;
    b[i].value = 4;
    c[i].value = 1;
    r[i].value = 4 * data[i].value + 2 * data[i + 1].value;
  }

  /*right segment*/
  a[N - 1].time = 2;
  b[N - 1].time = 7;
  c[N - 1].time = 0;
  r[N - 1].time = 8 * data[N - 1].time + data[N].time;

  a[N - 1].value = 2;
  b[N - 1].value = 7;
  c[N - 1].value = 0;
  r[N - 1].value = 8 * data[N - 1].value + data[N].value;

  /*solves Ax=b with the Thomas algorithm (from Wikipedia)*/
  for (int i = 1; i < N; i++)
  {
    double m;

    m = a[i].time / b[i - 1].time;
    b[i].time = b[i].time - m * c[i - 1].time;
    r[i].time = r[i].time - m * r[i - 1].time;

    m = a[i].value / b[i - 1].value;
    b[i].value = b[i].value - m * c[i - 1].value;
    r[i].value = r[i].value - m * r[i - 1].value;
  }

  _p1Points[N - 1].time = r[N - 1].time / b[N - 1].time;
  _p1Points[N - 1].value = r[N - 1].value / b[N - 1].value;
  for (int i = N - 2; i >= 0; --i)
  {
    _p1Points[i].time = (r[i].time - c[i].time * _p1Points[i + 1].time) / b[i].time;
    _p1Points[i].value = (r[i].value - c[i].value * _p1Points[i + 1].value) / b[i].value;
  }

  /*we have p1, now compute p2*/
  for (int i = 0; i < N - 1; i++)
  {
    _p2Points[i].time = 2 * data[i + 1].time - _p1Points[i + 1].time;
    _p2Points[i].value = 2 * data[i + 1].value - _p1Points[i + 1].value;
  }

  _p2Points[N - 1].time = 0.5 * (data[N].time + _p1Points[N - 1].time);
  _p2Points[N - 1].value = 0.5 * (data[N].value + _p1Points[N - 1].value);

  return true;
}