#ifndef ROBOT_H
#define ROBOT_H

#include "Entity.h"

class Robot : public Entity
{
    double d_lifetime;

public:
    Robot();
    virtual ~Robot();

    virtual void Sense(double dt)
    {
        d_lifetime += dt;
    }

    virtual int Think()
    {
        if (d_lifetime >= 120.0)
            return 1;
        else if (1)
            return 2;

        return 0;
    }

    virtual void Act(int value)
    {
        switch (value)
        {
        case 0:
            //ActShutDown();
            break;

        case 1:
            //ActCheer();
            break;
        }
    }
};

#endif