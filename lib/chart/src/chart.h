#ifndef CHART_H
#define CHART_H

#include <statistics.h>

#ifdef ARDUINO
#include <GxEPD.h>
#include <GxGDEW042Z15/GxGDEW042Z15.h> // 4.2" b/w/r
#endif

using namespace std;

class Chart
{
private:
public:
    Chart(void);
#ifdef ARDUINO
    void lineChart(GxEPD_Class *display, Timeseries *timeseries, uint16_t width, uint16_t height);
#endif
};
#endif