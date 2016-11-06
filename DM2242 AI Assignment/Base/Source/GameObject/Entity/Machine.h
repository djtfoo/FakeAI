#ifndef MACHINE_H
#define MACHINE_H

#include "Entity.h"

class Machine : public Entity
{
public:
    enum ROBOT_PART
    {
        HEAD,
        BODY,
        LIMB,
        MICROCHIP,
    };

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
    int m_maxScrapQuantity;

    MACHINE_STATE m_state;

    virtual void Sense(double dt);
    virtual int Think();
    virtual void Act(int value);

    virtual void Init();

    // Machine Variables & Functions
    ROBOT_PART m_partToCreate;

    void CreatePart();

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