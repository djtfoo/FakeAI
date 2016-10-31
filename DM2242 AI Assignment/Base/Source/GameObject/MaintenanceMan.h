#ifndef MAINTENANCE_MAN_H
#define MAINTENANCE_MAN_H

#include "Entity.h"

class MaintenanceMan : public Entity
{
    double d_breakCharge;

public:
    MaintenanceMan();
    virtual ~MaintenanceMan();

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