#include <uptime.h>

Uptime::Uptime(void)
{
}

uint32_t Uptime::getSeconds(void)
{
    timeval curTime;
    gettimeofday(&curTime, NULL);
    return curTime.tv_sec;
};

uint32_t Uptime::getMicros()
{
    timeval curTime;
    gettimeofday(&curTime, NULL);
    return curTime.tv_usec;
};

bool Uptime::setTime(tm time)
{
    time_t t = mktime(&time);
    struct timeval now = {.tv_sec = t};
    settimeofday(&now, NULL);
    return true;
}

tm *Uptime::getTime(void)
{
    time_t nowtime = getSeconds();
    return localtime(&nowtime);
}

bool Uptime::parseModemTime(String modemTime)
{
    tm tm;
    if (modemTime.length() < 21)
        return false;

    // example: "0,2020/07/05,06:21:02"
    try
    {
        String year = modemTime.substring(2, 6);
        tm.tm_year = year.toInt() - 1900;
        String month = modemTime.substring(7, 9);
        tm.tm_mon = month.toInt();
        String day = modemTime.substring(10, 12);
        tm.tm_mday = day.toInt();
        String hour = modemTime.substring(13, 15);
        tm.tm_hour = hour.toInt();
        String minute = modemTime.substring(16, 18);
        tm.tm_min = minute.toInt();
        String second = modemTime.substring(19, 21);
        tm.tm_sec = second.toInt();
        Serial.printf("[  TIME  ] Setting time: '%s' => %02d.%02d.%04d %02d:%02d:%02d\n", modemTime.c_str(), tm.tm_mday, tm.tm_mon, tm.tm_year + 1900, tm.tm_hour, tm.tm_min, tm.tm_sec);

        return setTime(tm);
    }
    catch (const std::exception &e)
    {
        Serial.printf("[ ERROR ] Uptime::parseModemTime(\"%s\"): %s\n", modemTime.c_str(), e.what());
        return false;
    }
}