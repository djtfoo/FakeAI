#ifndef ROBOT_H
#define ROBOT_H

#include "Entity.h"

class Robot : public Entity
{
public:
    enum ROBOT_STATE
    {
        STARTUP,
        WORK,
        CHEER,
        SHUTDOWN,
    };

private:
    double m_lifetime;
    
    ROBOT_STATE m_state;

public:
    Robot();
    virtual ~Robot();

    virtual void Sense(double dt);
    virtual int Think();
    virtual void Act(int value);
};

#endif