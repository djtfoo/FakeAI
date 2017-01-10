#ifndef TIME_H
#define TIME_H

#include <string>

struct Time
{
    enum DAY
    {
        MONDAY,
        TUESDAY,
        WEDNESDAY,
        THURSDAY,
        FRIDAY,
        SATURDAY,
        SUNDAY,
        
        DAYS_TOTAL
    };

private:
    DAY m_day;
    double m_seconds;  // the time in seconds; max is 

public:
    Time();
    ~Time();

    std::string GetDay();
    double GetSeconds();
};

#endif