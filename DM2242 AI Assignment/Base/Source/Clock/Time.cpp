#include "Time.h"
#include <sstream>

Time::Time(DAY day, double seconds)
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

    return "";
}

std::string Time::GetDayAbbreviation()
{
    switch (m_day)
    {
    case MONDAY: return "MON";
    case TUESDAY: return "TUE";
    case WEDNESDAY: return "WED";
    case THURSDAY: return "THU";
    case FRIDAY: return "FRI";
    case SATURDAY: return "SAT";
    case SUNDAY: return "SUN";
    }

    return "";
}

std::string Time::GetTime()
{
    int hour = m_seconds / 3600;
    int minutes = (m_seconds - (hour * 3600)) / 60;

    std::stringstream output;
    if (hour < 10)
        output << "0";
    output << hour << ":";
    if (minutes < 10)
        output << "0";
    output << minutes;
    
    return output.str();
}

//double Time::GetSeconds()
//{
//    return m_seconds;
//}

void Time::Update(const double dt)
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