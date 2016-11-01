#include "Entity.h"

Entity::Entity()
{
}

Entity::~Entity()
{
}

void Entity::RunFSM(double dt)
{
    Sense(dt);

    int thinkValue = Think();

    if (thinkValue)
    {
        Act(thinkValue);
    }
}