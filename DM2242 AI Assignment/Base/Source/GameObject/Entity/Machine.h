#ifndef MACHINE_H
#define MACHINE_H

#include "Entity.h"
#include "../ConveyorBelt.h"
#include "../RobotParts.h"

class Machine : public Entity
{
public:

    enum MACHINE_STATE
    {
        NIL,
        REST,
        PRODUCTION,
        WAITFORREFILL,
        BROKEN,
    };

private:
    double m_timer;
    double m_storedTimer;
    double m_overheatCharge;
    int m_scrapQuantity;
    int m_maxScrapQuantity;
	bool m_partCreated;
	bool m_isBroken;
    bool m_isEmpty;

    MACHINE_STATE m_state;

    // Location to spawn robot parts
    ConveyorBelt* m_SpawnLocation;

    virtual void Sense(double dt);
    virtual int Think();
    virtual void Act(int value);

    // Machine Variables & Functions
    RobotPart::ROBOT_PART m_partToCreate;

    void CreatePart();

public:
    Machine();
    virtual ~Machine();

    virtual void Init();
    virtual void Update(double dt);

    void SetPartToCreate(RobotPart::ROBOT_PART part);
    void SetSpawnLocation(ConveyorBelt* location);

    double GetTimer();
    double GetOverheatCharge();
    int GetScrapQuantity();
    bool IsBroken();
    bool IsEmpty();

	void SetIsBroken(bool status);
    void SetIsEmpty(bool status);

    MACHINE_STATE GetState();
};

#endif