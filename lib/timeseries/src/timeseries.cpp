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

float Timeseries::gauss(float sigma, float x)
{
  float expVal = -1 * (x * x) / (2 * sigma * sigma);
  float divider = 2 * PI * sigma * sigma;
  return exp(expVal)/divider;
}

void Timeseries::calulateKernel(int samples, float sigma)
{

  float sum = 0., val = 0.;
  bool doubleCenter = false;
  if (samples % 2 == 0)
  {
    doubleCenter = true;
    samples--;
  }

  int16_t steps = (samples - 1) / 2;
  float stepSize = (3 * sigma) / steps;

  kernel.clear();

  for (int i = steps; i >= 1; i--)
  {
    val = gauss(sigma, i * stepSize * -1);
    sum = sum + val;
    kernel.push_back(val);
  }

  val = gauss(sigma, 0);
  sum += val;
  kernel.push_back(val);
  if (doubleCenter)
  {
    val = gauss(sigma, 0);
    sum += val;
    kernel.push_back(val);
  }

  for (int i = 1; i <= steps; i++)
  {
    val = gauss(sigma, i * stepSize);
    sum += val;
    kernel.push_back(val);
  }

  for (float &i : kernel)
    i /= sum;
}


void Timeseries::applyFilter(uint8_t samples, float sigma)
{
  vector<Point> out;

    calulateKernel(samples, sigma);
    int32_t sampleSide = samples / 2;
    uint32_t ubound = data.size();
    for (uint32_t i = 0; i < ubound; i++) {
        float sample = 0;
        int32_t sampleCtr = 0;
        for (long j = i - sampleSide; j <= i + sampleSide; j++) {
            if (j > 0 && j < ubound) {
                int32_t sampleWeightIndex = sampleSide + (j - i);
                sample += kernel[sampleWeightIndex] * data[j].value;
                sampleCtr++;
            }
        }
        float smoothed = sample / (float)sampleCtr;
        out.push_back(Point({data.at(i).time, smoothed}));
    }

  data.assign(out.begin(), out.end());
}
