#ifndef MACHINE_H
#define MACHINE_H

#include "Entity.h"

class Machine : public Entity
{
public:
    enum ROBOT_PART
    {
        HEAD = 2,
        BODY = 4,
        LIMB = 3,
        MICROCHIP = 1,
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
	bool m_partCreated;
	bool m_isBroken;

    MACHINE_STATE m_state;

    virtual void Sense(double dt);
    virtual int Think();
    virtual void Act(int value);

    // Machine Variables & Functions
    ROBOT_PART m_partToCreate;

    void CreatePart();

public:
    Machine();
    virtual ~Machine();

    virtual void Init();
    virtual void Update(double dt);

    void SetPartToCreate(ROBOT_PART part);

    double GetTimer();
    double GetOverheatCharge();
    int GetScrapQuantity();
    bool IsBroken();
    bool IsEmpty();

	void SetIsBroken(bool status);
};

#endif