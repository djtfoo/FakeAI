#ifndef CLOCK_H
#define CLOCK_H

#include "Time.h"

class Clock
{
    Time m_time;

public:
    Clock(Time time);
    ~Clock();

    void Update(const double dt);
};

#endif