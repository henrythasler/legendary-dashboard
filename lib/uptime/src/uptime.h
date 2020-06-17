#ifndef UPTIME_H
#define UPTIME_H

#include <stdint.h>
#include <stddef.h>
#include <sys/time.h>

using namespace std;

class Uptime
{
private:

public:
    Uptime(void);
    uint32_t getSeconds(void);
};
#endif