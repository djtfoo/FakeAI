#include "GL\glew.h"
#include "../Utility.h"

#include "Toilet.h"
#include "Entity\Worker.h"

Toilet::Toilet() : GameObject("Toilet")
{
}

Toilet::~Toilet()
{
}

void Toilet::Init()
{
    b_occupied = false;
    m_change = false;
}

void Toilet::Update(double dt)
{

}

bool Toilet::IsOccupied()
{
    return b_occupied;
}

void Toilet::SetOccupied(bool status)
{
    SetSprite();
    b_occupied = status;

    if (status == false)
        m_change = true;
    else
        m_change = false;
}

void Toilet::SetSprite()
{
    if (this->m_mesh->textureID > 0)
    {
        float texCountHeight;

        if (b_occupied)
            texCountHeight = 0;
        else
            texCountHeight = 1;
        
        float texHeight = 1.f * 0.5f;

        TexCoord texCoords[4] = {
            TexCoord(0, texHeight * texCountHeight),
            TexCoord(1, texHeight * texCountHeight),
            TexCoord(1, texHeight * (texCountHeight + 1) - 0.01f),
            TexCoord(0, texHeight * (texCountHeight + 1) - 0.01f)
        };

        glBindBuffer(GL_ARRAY_BUFFER, m_mesh->vertexBuffer);
        for (unsigned i = 0; i < 4; ++i) {
            glBufferSubData(GL_ARRAY_BUFFER, (sizeof(Vertex)-sizeof(TexCoord)) + (i * sizeof(Vertex)), sizeof(TexCoord), &texCoords[i]);
        }
    }
}


Vector3 Toilet::GetQueuePosition(int idx)
{
    return Vector3(m_pos.x, m_pos.y - idx, m_pos.z);
}

int Toilet::AddToQueue(GameObject* object)
{
    m_ToiletQueue.push(object);
    return m_ToiletQueue.size() - 1;

    //m_ToiletVec.push_back(object);
    //return m_ToiletVec.size() - 1;
}

void Toilet::RemoveFromQueue()
{
    m_ToiletQueue.pop();
    //int size = m_ToiletVec.size();
    //m_ToiletVec.erase(m_ToiletVec.begin(), m_ToiletVec.begin()+1);
    //m_ToiletVec.resize(size-1);
}

int Toilet::GetToiletSize()
{
    //return m_ToiletVec.size() - 1;
    return m_ToiletQueue.size();
}

bool Toilet::CheckIfChange()
{
    return m_change;
}