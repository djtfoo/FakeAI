#ifndef MACHINE_H
#define MACHINE_H

#include "Entity.h"

class Machine : public Entity
{
    double d_overheatCharge;

public:
    Machine();
    virtual ~Machine();

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