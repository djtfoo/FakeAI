#ifndef ROBOT_H
#define ROBOT_H

#include "Entity.h"

class Robot : public Entity
{
public:
    enum ROBOT_STATE
    {
        INCOMPLETE1, // body //being built in progress
        INCOMPLETE2, // body + limbs
        INCOMPLETE3, // body + limbs + head

        STARTUP,
        WORK,
        CHEER,
        SHUTDOWN,
    };

private:
    double m_lifetime;
    
    ROBOT_STATE m_state;

    virtual void Sense(double dt);
    virtual int Think();
    virtual void Act(int value);

public:
    Robot();
    virtual ~Robot();

    void SetRobotState(ROBOT_STATE state);
    ROBOT_STATE GetRobotState();

    virtual void Init();
    virtual void Update(double dt);
};

#endif