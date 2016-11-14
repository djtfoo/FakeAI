#include "GameObject.h"

std::vector<GameObject*> GameObject::m_goList;

GameObject::GameObject(std::string name, bool isEntity) : m_name(name), b_isEntity(isEntity)
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

bool GameObject::IsEntity()
{
    return b_isEntity;
}

void GameObject::SetPos(const Vector3& pos)
{
    this->m_pos = pos;
}

void GameObject::SetMesh(Mesh* mesh)
{
    this->m_mesh = mesh;
}

void GameObject::SetActive()
{
    this->b_active = true;
}

void GameObject::SetInactive()
{
    this->b_active = false;
}