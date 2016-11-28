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
        //NIL,
        REST,
        PRODUCTION,
        WAITFORREFILL,
        BROKEN,
        MACHINE_STATES_TOTAL
    };

private:
    double m_timer;
    double m_storedTimer;
	double m_overheatTimer;
    double m_overheatCharge;
    int m_scrapQuantity;
    int m_maxScrapQuantity;
	bool m_partCreated;
	bool m_isBroken;
    bool m_isEmpty;
    bool m_isBeingWorkedOn;

    MACHINE_STATE m_state;

    // Location to spawn robot parts
    Vector3 m_SpawnLocation;
    //ConveyorBelt* m_SpawnLocation;

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
    int GetPartToCreate();
	void SetSpawnLocation(const Vector3& location);

    double GetTimer();
    double GetOverheatCharge();
    int GetScrapQuantity();
    int GetMaxScrapQuantity();
    bool IsBroken();
    bool IsEmpty();

	void SetIsBroken(bool status);
    void SetIsEmpty(bool status);

    void SetIsBeingWorkedOn(bool status);
    bool GetIsBeingWorkedOn();

    MACHINE_STATE GetState();
    void SetState(MACHINE_STATE state);
    virtual int GetStateInt();
    virtual int GetMaxStates();

    void Refill();

    // DEBUG UI
    float randNum;
};

#endif