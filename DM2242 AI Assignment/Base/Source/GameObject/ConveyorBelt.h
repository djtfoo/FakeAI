#ifndef CONVEYOR_BELT_H
#define CONVEYOR_BELT_H

#include "GameObject.h"

class ConveyorBelt : public GameObject
{
public:
    ConveyorBelt();
    ~ConveyorBelt();

    virtual void Init();
    virtual void Update(double dt);

private:

};

#endif