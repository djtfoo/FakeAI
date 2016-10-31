#ifndef SCRAP_MAN_H
#define SCRAP_MAN_H

#include "Entity.h"

class ScrapMan : public Entity
{
    double d_breakCharge;

public:
    ScrapMan();
    virtual ~ScrapMan();

    virtual void Sense(double dt)
    {

    }

    virtual int Think()
    {


        return 0;
    }

    virtual void Act(int value)
    {
        switch (value)
        {
        case 0:
            break;

        case 1:
            break;
        }
    }
};

#endif