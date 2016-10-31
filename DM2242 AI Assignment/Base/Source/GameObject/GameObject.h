#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <string>
#include "Vector3.h"

class GameObject
{
protected:
    GameObject();

public:
    virtual ~GameObject();

    std::string m_name;
    Vector3 m_pos;
    bool b_active;
};

#endif