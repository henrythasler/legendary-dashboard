#include <chart.h>

Chart::Chart(void)
{
}

#ifdef ARDUINO
void Chart::lineChart(GxEPD_Class *display,
                      Timeseries *timeseries,
                      uint16_t canvasLeft,
                      uint16_t canvasTop,
                      uint16_t canvasWidth,
                      uint16_t canvasHeight,
                      uint16_t lineColor,
                      bool drawDataPoints,
                      bool yAxisMinAuto,
                      bool yAxisMaxAuto,
                      float yAxisMin,
                      float yAxisMax)
{
    if (timeseries->data.size() > 1)
    {
        uint32_t tMin = timeseries->data.front().first;
        uint32_t tMax = timeseries->data.back().first;

        float dataMin = yAxisMinAuto ? timeseries->min : yAxisMin;
        float dataMax = yAxisMaxAuto ? timeseries->max : yAxisMax;
        float dataOffset = (dataMax + dataMin) / 2.;

        float pixelPerTime = float(canvasWidth) / max(float(tMax - tMin), float(1.));
        float pixelPerValue = float(canvasHeight) / (dataMax - dataMin + 0.01);

        // Serial.printf("[  CHART ] tMin=%u tMax=%u pixelPerTime=%f pixelPerValue=%f\n", tMin, tMax, pixelPerTime, pixelPerValue);

        uint16_t screenX1 = 0, screenY1 = 0, screenX2 = 0, screenY2 = 0;

        float t1 = 0, y1 = 0, t2 = 0, y2 = 0;
        for (vector<Point>::const_iterator i = timeseries->data.begin() + 1; i != timeseries->data.end(); ++i)
        {
            t1 = float(Point(*(i - 1)).first);
            y1 = Point(*(i - 1)).second - dataOffset;

            screenX1 = canvasLeft + int16_t((t1 - tMin) * pixelPerTime);
            screenY1 = canvasTop + canvasHeight / 2 - int16_t(y1 * pixelPerValue);

            t2 = float(Point(*i).first);
            y2 = Point(*i).second - dataOffset;

            screenX2 = canvasLeft + int16_t((t2 - tMin) * pixelPerTime);
            screenY2 = canvasTop + canvasHeight / 2 - int16_t(y2 * pixelPerValue);

            display->drawLine(screenX1, screenY1, screenX2, screenY2, lineColor);
            if (drawDataPoints)
                display->fillCircle(screenX1, screenY1, 2, lineColor);
            // Serial.printf("[  CHART ] t1=%.4f, y1=%.4f => (%u, %u)\n", t1, y1, screenX1, screenY1);
        }
    }

    // for (int x = 0; x <= 80; x++)
    //     display->drawPixel(x, 150 - cos((float(x) / 20.) * (float(x) / 20.) - 1) * 20, GxEPD_BLACK);
}
#endif