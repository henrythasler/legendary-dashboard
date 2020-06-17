#include <uptime.h>

Uptime::Uptime(void)
{
}

uint32_t Uptime::getSeconds()
{
    timeval curTime;
    gettimeofday(&curTime, NULL);
    return curTime.tv_sec;
};