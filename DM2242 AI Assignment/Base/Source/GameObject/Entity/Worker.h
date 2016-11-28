#ifndef WORKER_H
#define WORKER_H

#include "Entity.h"
#include "../Workstation.h"
#include "Pathfinder.h"
#include "../Toilet.h"

class Worker : public Entity
{
public:
    enum WORKER_STATE
    {
        IDLE,
        WORK,
        BREAK,
        WORKER_STATES_TOTAL
    };

private:

    double m_breakCharge;
    bool m_workCompleted;
    bool m_inToilet;
    bool m_breakDone;
    bool m_atWorkstation;
    bool m_doOnce;
    int m_toiletIdx;
    Vector3 m_origSpawn; 

    Workstation* m_workstation;
    Toilet* m_toilet;

    Pathfinder* m_pathfinder;

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
    bool IsAbleToWork();
    bool IsOnBreak();

    void SetWorkstation(Workstation* station);

    virtual void Init();
    virtual void Update(double dt);
    virtual void SetPos(Vector3 pos);

    WORKER_STATE GetState();
    void SetState(WORKER_STATE state);
    virtual int GetStateInt();
    virtual int GetMaxStates();

    void SetToilet(Toilet* toilet);
    Toilet* GetToilet();

    double GetBreakCharge();

    double m_timer;
};

#endif