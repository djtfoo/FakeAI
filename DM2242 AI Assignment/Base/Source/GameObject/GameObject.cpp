#include "GameObject.h"

GameObject::GameObject()
{
}

GameObject::~GameObject()
{
}

std::string GameObject::GetName()
{
    return m_name;
}

Vector3 GameObject::GetPos()
{
    return m_pos;
}

bool GameObject::IsActive()
{
    return b_active;
}

Mesh* GameObject::GetMesh()
{
    return m_mesh;
}