#ifndef WORKER_H
#define WORKER_H

#include "Entity.h"
#include "../Workstation.h"

class Worker : public Entity
{
public:
    enum WORKER_STATE
    {
        IDLE,
        WORK,
        BREAK,
    };

private:
    double m_timer;
    double m_breakCharge;
    Workstation* m_workstation;

    WORKER_STATE m_state;

    virtual void Sense(double dt);
    virtual int Think();
    virtual void Act(int value);

public:
    Worker();
    virtual ~Worker();

    bool IsPartAtWorkstation();
    bool IsOnBreak();
};

#endif