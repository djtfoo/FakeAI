#ifndef BUILDINGBLOCK_STACK_H
#define BUILDINGBLOCK_STACK_H

#include "GameObject.h"

class BuildingBlockStack : public GameObject
{
public:
    BuildingBlockStack();
    virtual ~BuildingBlockStack();

    virtual void Init();
    virtual void Update(double dt);
};

#endif