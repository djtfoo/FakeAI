#ifndef MACHINE_H
#define MACHINE_H

#include "Entity.h"

class Machine : public Entity
{
public:
    enum MACHINE_STATE
    {
        REST,
        PRODUCTION,
        WAITFORREFILL,
        BROKEN,
    };

private:
    double m_timer;
    double m_overheatCharge;
    int m_scrapQuantity;

    MACHINE_STATE m_state;

    virtual void Sense(double dt);
    virtual int Think();
    virtual void Act(int value);

public:
    Machine();
    virtual ~Machine();

    double GetTimer();
    double GetOverheatCharge();
    int GetScrapQuantity();
    bool IsBroken();
    bool IsEmpty();
};

#endif