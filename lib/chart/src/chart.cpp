#include <chart.h>

Chart::Chart(void)
{
}

#ifdef ARDUINO
void Chart::lineChart(GxEPD_Class *display, Timeseries *timeseries, uint16_t width, uint16_t height)
{
    uint32_t tMin = timeseries->data.front().first;
    uint32_t tMax = timeseries->data.back().first;

    // float yMin = 

    float dt = float(width) / float(tMax - tMin);
    float dy = float(height) / (timeseries->max - timeseries->min);

    Serial.printf("[  CHART ] tMin=%u tMax=%u dt=%f dy=%f\n", tMin, tMax, dt, dy);

    uint16_t screenX = 0, screenY=0;

    for (vector<Point>::const_iterator i = timeseries->data.begin(); i != timeseries->data.end(); ++i)
    {
        float t = float(Point(*i).first);
        float y = Point(*i).second;

        screenX = int16_t((t-tMin) * dt);
        screenY = height / 2 - int16_t(y * dy);

        display->fillCircle(screenX, screenY, 3, GxEPD_BLACK);
        Serial.printf("[  CHART ] t=%.4f, y=%.4f => (%u, %u)\n", t, y, screenX, screenY);
    }

    // for (int x = 0; x <= 80; x++)
    //     display->drawPixel(x, 150 - cos((float(x) / 20.) * (float(x) / 20.) - 1) * 20, GxEPD_BLACK);
}
#endif