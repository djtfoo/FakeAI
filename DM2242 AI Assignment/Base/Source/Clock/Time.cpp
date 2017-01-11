#include "Time.h"
#include <sstream>

Time::Time(DAY day = MONDAY, double seconds = 0.0)
: m_day(day)
, m_seconds(seconds)
{
}

Time::~Time()
{
}

std::string Time::GetDay()
{
    switch (m_day)
    {
    case MONDAY: return "Monday";
    case TUESDAY: return "Tuesday";
    case WEDNESDAY: return "Wednesday";
    case THURSDAY: return "Thursday";
    case FRIDAY: return "Friday";
    case SATURDAY: return "Saturday";
    case SUNDAY: return "Sunday";
    }
}

std::string Time::GetTime()
{
    int hour = m_seconds / 3600;
    int minutes = (m_seconds - (hour * 3600)) / 60;

    std::stringstream output;
    output << hour << ":" << minutes;
    
    return output.str();
}

//double Time::GetSeconds()
//{
//    return m_seconds;
//}

void Time::IncreaseTime(const double dt)
{
    m_seconds += dt;
    if (IsNewDay())
    {
        m_seconds -= 86400;
        
        m_day = static_cast<Time::DAY>(m_day + 1);
        if (m_day == DAYS_TOTAL)    // Sunday transitioning to Monday
            m_day = MONDAY;
    }
}

bool Time::IsNewDay()
{
    return (m_seconds >= 86400);
}