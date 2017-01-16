#ifndef SUPERVISOR_H
#define SUPERVISOR_H

#include "Entity.h"
#include "../Workstation.h"
#include "../Toilet.h"

class Supervisor : public Entity
{
public:
    enum SUPERVISOR_STATE
    {
        IDLE,
        PATROL,
        BREAK,
        MAKEDECISION,
        OFFWORK,
        SUPERVISOR_STATES_TOTAL
    };

private:

    double m_breakCharge;
    bool m_inToilet;
    bool m_breakDone;
    bool m_atWorkstation;
    bool m_doOnce;
    bool m_shouldMoveForward;
    int m_toiletIdx;
    Vector3 m_origSpawn;

    Workstation* m_workstation;
    Toilet* m_toilet;

    //Pathfinder* m_pathfinder;
    //bool b_reachedDestination;

    SUPERVISOR_STATE m_state;

    void DoIdle();
    void DoPatrol();
    void DoBreak();
    void DoMakeDecision();
    void DoOffWork();

    virtual void Sense(double dt);
    virtual int Think();
    virtual void Act(int value);


public:
    Supervisor();
    virtual ~Supervisor();

    bool IsOnBreak();

    void SetWorkstation(Workstation* station);

    virtual void Init();
    virtual void Update(double dt);
    virtual void SetPos(Vector3 pos);

    SUPERVISOR_STATE GetState();
    void SetState(SUPERVISOR_STATE state);
    virtual int GetStateInt();
    virtual int GetMaxStates();

    void SetToilet(Toilet* toilet);
    Toilet* GetToilet();

    double m_timer;

    // Getters
    double GetBreakCharge();
    Pathfinder* GetPathfinder();
};

#endif