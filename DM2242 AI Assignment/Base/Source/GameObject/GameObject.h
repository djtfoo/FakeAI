#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <string>
#include "Vector3.h"
#include "../Mesh.h"

class GameObject
{
protected:
    std::string m_name;
    Vector3 m_pos;
    bool b_active;
    Mesh* m_mesh;

    GameObject();

public:
    virtual ~GameObject();

    std::string GetName();
    Vector3 GetPos();
    bool IsActive();
    Mesh* GetMesh();
};

#endif