#include <chart.h>

Chart::Chart(void)
{
}

#ifdef ARDUINO
void Chart::lineChart(GxEPD_Class *display, Timeseries *timeseries, uint16_t posX, uint16_t posY, uint16_t width, uint16_t height, uint16_t lineColor)
{
    if (timeseries->data.size() > 1)
    {
        uint32_t tMin = timeseries->data.front().first;
        uint32_t tMax = timeseries->data.back().first;

        // float yMin =

        float dt = float(width) / float(tMax - tMin);
        float dy = float(height) / (timeseries->max - timeseries->min);

        Serial.printf("[  CHART ] tMin=%u tMax=%u dt=%f dy=%f\n", tMin, tMax, dt, dy);

        uint16_t screenX1 = 0, screenY1 = 0, screenX2 = 0, screenY2 = 0;
        float t1 = 0, y1 = 0, t2 = 0, y2 = 0;
        for (vector<Point>::const_iterator i = timeseries->data.begin() + 1; i != timeseries->data.end(); ++i)
        {
            t1 = float(Point(*(i - 1)).first);
            y1 = Point(*(i - 1)).second;

            screenX1 = posX + int16_t((t1 - tMin) * dt);
            screenY1 = posY + height / 2 - int16_t(y1 * dy);

            t2 = float(Point(*i).first);
            y2 = Point(*i).second;

            screenX2 = posX + int16_t((t2 - tMin) * dt);
            screenY2 = posY + height / 2 - int16_t(y2 * dy);

            display->drawLine(screenX1, screenY1, screenX2, screenY2, lineColor);
            display->fillCircle(screenX1, screenY1, 2, lineColor);
            // Serial.printf("[  CHART ] t1=%.4f, y1=%.4f => (%u, %u)\n", t1, y1, screenX1, screenY1);
        }
    }

    // for (int x = 0; x <= 80; x++)
    //     display->drawPixel(x, 150 - cos((float(x) / 20.) * (float(x) / 20.) - 1) * 20, GxEPD_BLACK);
}
#endif