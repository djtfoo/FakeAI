#include "Clock.h"

Clock::Clock(Time time) : m_time(time)
{
}

Clock::~Clock()
{
}

void Clock::Update(const double dt)
{
    m_time.IncreaseTime(dt);
}