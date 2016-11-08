#include "Entity.h"

Entity::Entity(std::string name) : GameObject(name, true), m_dir(DIR_DOWN)
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