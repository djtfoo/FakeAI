#ifndef MAINTENANCE_MAN_H
#define MAINTENANCE_MAN_H

#include "Entity.h"

class MaintenanceMan : public Entity
{
public:
    enum MAINTENANCEMAN_STATE
    {
        IDLE,
        REPAIR,
        REFILL,
        BREAK,
    };

private:
    double d_breakCharge;

    MAINTENANCEMAN_STATE m_state;

public:
    MaintenanceMan();
    virtual ~MaintenanceMan();

    virtual void Sense(double dt);
    virtual int Think();
    virtual void Act(int value);
};

#endif