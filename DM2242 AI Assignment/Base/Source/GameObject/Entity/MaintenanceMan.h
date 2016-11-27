#ifndef MAINTENANCE_MAN_H
#define MAINTENANCE_MAN_H

#include "Entity.h"
#include "Machine.h"
#include "../Workstation.h"
#include "../Toilet.h"

class MaintenanceMan : public Entity
{
public:
  
    enum MAINTENANCEMAN_STATE
    {
        IDLE,
        REPAIR,
        REFILL,
        BREAK,
        MAINTENANCEMAN_STATES_TOTAL
    };

private:
    double m_breakCharge;
    double m_timer;

    MAINTENANCEMAN_STATE m_state;

    Machine* m_targetMachine;

    bool m_doingWork;
    bool m_breakDone;
    int m_toiletIdx;

    int ScanMachines();
    void DoIdle();
    void DoRepair();
    void DoRefill();
    void DoBreak();

    virtual void Sense(double dt);
    virtual int Think();
    virtual void Act(int value);

    Workstation* m_workstation;
    Toilet* m_toilet;

public:
    MaintenanceMan();
    virtual ~MaintenanceMan();

    virtual void Init();
    virtual void Update(double dt);

    MAINTENANCEMAN_STATE GetState();
    void SetState(MAINTENANCEMAN_STATE state);
    virtual int GetStateInt();
    virtual int GetMaxStates();

    void SetWorkstation(Workstation* station);

    void SetToilet(Toilet* toilet);
    Toilet* GetToilet();
};

#endif