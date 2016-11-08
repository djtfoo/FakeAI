#ifndef ROBOT_H
#define ROBOT_H

#include "Entity.h"

class Robot : public Entity
{
public:
    enum ROBOT_STATE
    {
        INCOMPLETE1, // being built in progress
        INCOMPLETE2,
        INCOMPLETE3,

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

    virtual void Init();
    virtual void Update(double dt);
};

#endif