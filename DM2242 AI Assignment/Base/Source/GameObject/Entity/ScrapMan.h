#ifndef SCRAP_MAN_H
#define SCRAP_MAN_H

#include "Entity.h"
#include "Pathfinder.h"
#include "../ScrapPile.h"
#include "Robot.h"
#include "../Toilet.h"

class ScrapMan : public Entity
{
public:
    enum SCRAPMAN_STATE
    {
        IDLE,
        COLLECT_ROBOT,
        BREAKDOWN_ROBOT,
        BREAK,
        
        SCRAPMAN_STATES_TOTAL
    };

private:
    double d_breakCharge;

    double d_timerCounter;  // for breaking down robot when back at workstation

    SCRAPMAN_STATE m_state;

    Robot* m_robotToPickUp;

    Pathfinder* m_pathfinder;
    bool b_reachedDestination;

    ScrapPile* m_pile;  // aka work place lol

    virtual void Sense(double dt);
    virtual int Think();
    virtual void Act(int value);

    // Toilet 
    bool m_breakDone;
    bool m_inToilet;
    bool m_doOnce;
    bool m_atWorkstation;
    int m_toiletIdx;
    Vector3 m_origSpawn;
    Toilet* m_toilet;

public:
    ScrapMan();
    virtual ~ScrapMan();

    virtual void Init();
    virtual void Update(double dt);
    virtual void SetPos(Vector3 pos);

    SCRAPMAN_STATE GetState();
    void SetState(SCRAPMAN_STATE state);
    virtual int GetStateInt();
    virtual int GetMaxStates();

    void AssignScrapPile(ScrapPile* pile);

    // Toilet 
    void SetToilet(Toilet* toilet);
    Toilet* GetToilet();
};

#endif