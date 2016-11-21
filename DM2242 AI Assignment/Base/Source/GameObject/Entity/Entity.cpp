#include "GL\glew.h"

#include "Entity.h"
#include "../../Utility.h"

Entity::Entity(std::string name) : GameObject(name, true), m_dir(DIR_DOWN)
{
}

Entity::~Entity()
{
    if (m_mesh)
    {
        delete m_mesh;
        m_mesh = NULL;
    }
}

void Entity::RunFSM(double dt)
{
    Sense(dt);

    int thinkValue = Think();

    if (thinkValue != -1)
    {
        Act(thinkValue);
        //SetSprite();
    }
}

void Entity::SetDirection(DIRECTION dir)
{
    this->m_dir = dir;
    //this->SetSprite();
}

void Entity::SetSprite()
{
    if (this->m_mesh->textureID > 0)
    {
        float texCountWidth = m_dir;
        float texCountHeight = GetMaxStates() - 1 - GetStateInt();
        float texWidth = 1.f / 4;
        float texHeight = 1.f / GetMaxStates();

        TexCoord texCoords[4] = {
            TexCoord(texWidth * texCountWidth, texHeight * texCountHeight),
            TexCoord(texWidth * (texCountWidth + 1), texHeight * texCountHeight),
            TexCoord(texWidth * (texCountWidth + 1), texHeight * (texCountHeight + 1) - 0.01f),
            TexCoord(texWidth * texCountWidth, texHeight * (texCountHeight + 1) - 0.01f)
        };

        glBindBuffer(GL_ARRAY_BUFFER, m_mesh->vertexBuffer);
        for (unsigned i = 0; i < 4; ++i) {
            glBufferSubData(GL_ARRAY_BUFFER, (sizeof(Vertex)-sizeof(TexCoord)) + (i * sizeof(Vertex)), sizeof(TexCoord), &texCoords[i]);
        }
    }
}

int Entity::GetStateInt()
{
    return 0;
}

int Entity::GetMaxStates()
{
    return 1;
}