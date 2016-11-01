#ifndef SCRAP_MAN_H
#define SCRAP_MAN_H

#include "Entity.h"

class ScrapMan : public Entity
{
public:
    enum SCRAPMAN_STATE
    {
        IDLE,
        PICKUP_ROBOT,
        BREAKDOWN_ROBOT,
        BREAK,
    };

private:
    double d_breakCharge;

    SCRAPMAN_STATE m_state;

public:
    ScrapMan();
    virtual ~ScrapMan();

    virtual void Sense(double dt);
    virtual int Think();
    virtual void Act(int value);
};

#endif