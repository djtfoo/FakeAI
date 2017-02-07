#ifndef ROBOT_H
#define ROBOT_H

#include "Entity.h"
#include "../ConveyorBelt.h"

class Robot : public Entity
{
public:
    enum ROBOT_STATE
    {
        INCOMPLETE_1, // body //being built in progress
        INCOMPLETE_2, // body + limbs
        INCOMPLETE_3, // body + limbs + head

        STARTUP,
        WORK_WITHOUTPART,
        WORK_WITHPART,
        CHEER,
        SHUTDOWN,
        POWEROFF,

        ROBOT_STATES_TOTAL
    };

private:
    double m_lifetime;
    double d_timerCounter;  // used for a pause when facing objects, as well as for cheer

    double d_timerForShutDown;
    bool b_toShutDown;

    //bool b_reachedLane; // maybe

    //Pathfinder* m_pathfinder;
    //bool b_reachedDestination;
    
    ROBOT_STATE m_state;

	int m_currWaypoint;
	ConveyorBelt* m_beltToFollow;

    bool b_workedOn;
    bool b_ornamentCompleted;
    ROBOT_STATE m_stateBeforeCheer;

    virtual void Sense(double dt);
    virtual int Think();
    virtual void Act(int value);


public:
    Robot();
    virtual ~Robot();

	void SetBelt(ConveyorBelt* belt);
	void SetWaypoint(int idx);
    Vector3 GetWaypoint();

    void SetWorkedOn(bool status);
    bool IsWorkedOn();

    virtual void Init();
    virtual void Update(double dt);

    ROBOT_STATE GetState();
    void SetState(ROBOT_STATE state);
    virtual int GetStateInt();
    virtual int GetMaxStates();

    // Getters
    double GetLifetime();
};

#endif