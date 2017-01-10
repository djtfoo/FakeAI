#include "Time.h"

Time::Time()
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

double Time::GetSeconds()
{
    return m_seconds;
}