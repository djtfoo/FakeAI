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
    double m_seconds;  // the time in seconds; max in a day is 86400 seconds

    bool IsNewDay();

public:
    Time(DAY day = MONDAY, double seconds = 0.0);
    ~Time();

    std::string GetDay();
    std::string GetDayAbbreviation();
    std::string GetTime();
    //double GetSeconds();

    void Update(const double dt);
};

#endif