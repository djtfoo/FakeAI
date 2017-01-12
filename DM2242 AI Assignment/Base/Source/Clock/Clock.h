#ifndef CLOCK_H
#define CLOCK_H

#include "Time.h"

class Clock
{
    Time m_time;

public:
    Clock(Time time);
    ~Clock();

    std::string GetCurrDay();
    std::string GetCurrDayAbbreviation();
    std::string GetCurrTime();
    void Update(const double dt);
};

#endif