#ifndef WORKER_H
#define WORKER_H

#include "Entity.h"
#include "../Workstation.h"

class Worker : public Entity
{
public:
    enum WORKER_STATE
    {
        NIL,
        IDLE,
        WORK,
        BREAK,
    };

private:
    double m_timer;
    double m_breakCharge;
    bool m_workCompleted;
    bool m_inToilet;
    bool m_atWorkstation;
    Workstation* m_workstation;

    WORKER_STATE m_state;

    void DoIdle();
    void DoWork();
    void DoBreak();

    virtual void Sense(double dt);
    virtual int Think();
    virtual void Act(int value);

public:
    Worker();
    virtual ~Worker();

    bool IsPartAtWorkstation();
    bool IsOnBreak();

    void SetWorkstation(Workstation* station);

    virtual void Init();
    virtual void Update(double dt);
};

#endif