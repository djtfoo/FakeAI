#ifndef ENTITY_H
#define ENTITY_H

#include "GameObject.h"

class Entity : public GameObject
{
protected:
    Entity();

public:
    virtual ~Entity();

    virtual void RunFSM(double dt)
    {
        Sense(dt);

        int thinkValue = Think();

        if (thinkValue)
        {
            Act(thinkValue);
        }
    }

    virtual void Sense(double dt) = 0;  // get/receive updates from the world
    virtual int Think() = 0;   // process the updates
    virtual void Act(int value) = 0;     // act upon any change in behaviour

    // idea:
    // multiple types of think and act... use int values for switch case
    // Act(Think());
};

#endif