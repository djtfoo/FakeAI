#ifndef DOOR_H
#define DOOR_H

#include "GameObject.h"

class Door : public GameObject
{
public:
    Door();
    virtual ~Door();

    virtual void Init();
    virtual void Update(double dt);
};

#endif