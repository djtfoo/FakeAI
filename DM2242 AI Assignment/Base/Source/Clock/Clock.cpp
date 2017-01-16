#include "Clock.h"

Clock::Clock(Time time) : m_time(time)
{
}

Clock::~Clock()
{
}

std::string Clock::GetCurrDay()
{
    return m_time.GetDay();
}

std::string Clock::GetCurrDayAbbreviation()
{
    return m_time.GetDayAbbreviation();
}

std::string Clock::GetCurrTime()
{
    return m_time.GetTime();
}

Time Clock::GetCurrTimeObject()
{
    return m_time;
}

int Clock::GetCurrMinutes()
{
    return m_time.GetMinutes();
}

int Clock::GetCurrHours()
{
    return m_time.GetHours();
}

void Clock::Update(const double dt)
{
    m_time.Update(dt);
}

bool Clock::GetIsWorkDay()
{
    // Day defined as 0800 - 1800
    if (m_time.GetHours() >= 8 && m_time.GetHours() <= 17 && (m_time.GetDayAbbreviation() != "SAT" && m_time.GetDayAbbreviation() != "SUN"))
        return true;
    else
        return false;
}

bool Clock::GetIsWorkStarted()
{
    if (m_time.GetHours() >= 9 && m_time.GetHours() <= 17)
        return true;
    else
        return false;
}