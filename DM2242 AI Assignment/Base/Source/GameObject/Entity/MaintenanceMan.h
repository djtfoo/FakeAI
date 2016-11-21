#ifndef MAINTENANCE_MAN_H
#define MAINTENANCE_MAN_H

#include "Entity.h"
#include "Machine.h"

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
    double m_breakCharge;
    double m_timer;

    MAINTENANCEMAN_STATE m_state;

    Machine* m_targetMachine;

    bool m_doingWork;

    int ScanMachines();
    void DoIdle();
    void DoRepair();
    void DoRefill();
    void DoBreak();

    virtual void Sense(double dt);
    virtual int Think();
    virtual void Act(int value);

public:
    MaintenanceMan();
    virtual ~MaintenanceMan();

    virtual void Init();
    virtual void Update(double dt);
};

#endif