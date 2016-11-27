#include "GL\glew.h"

#include "GameObject.h"
#include "Entity/Entity.h"
#include "../MeshBuilder.h"

GameObject::GameObject(std::string name, bool isEntity) : m_name(name), b_isEntity(isEntity), m_scale(1, 1, 1), b_active(true)
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

Vector3 GameObject::GetScale()
{
    return m_scale;
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

void GameObject::SetMesh(Mesh* mesh, bool wantNewMesh)
{
    /*this->m_mesh = mesh;
    if (this->b_isEntity)
    {
        Entity* entity = dynamic_cast<Entity*>(this);
        entity->SetSprite();
    }*/

    if (this->b_isEntity || wantNewMesh)
    {
        this->m_mesh = MeshBuilder::GenerateQuad(this->GetName(), Color(1, 0.5f, 0));
        this->m_mesh->textureID = mesh->textureID;
    }
    else
    {
        this->m_mesh = mesh;
    }

    if (this->m_name != "ConveyorBelt")
        this->SetSprite();
}

void GameObject::SetActive()
{
    this->b_active = true;
}

void GameObject::SetInactive()
{
    this->b_active = false;
}

void GameObject::SetSprite()
{
    if (this->m_mesh->textureID > 0)
    {
        TexCoord texCoords[4] = {
            TexCoord(0.f, 0.f),
            TexCoord(1.f, 0.f),
            TexCoord(1.f, 1.f),
            TexCoord(0.f, 1.f)
        };

        glBindBuffer(GL_ARRAY_BUFFER, m_mesh->vertexBuffer);
        for (unsigned i = 0; i < 4; ++i) {
            glBufferSubData(GL_ARRAY_BUFFER, (sizeof(Vertex)-sizeof(TexCoord)) + (i * sizeof(Vertex)), sizeof(TexCoord), &texCoords[i]);
        }
    }
}