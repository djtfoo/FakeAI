#include "GL\glew.h"
#include "../Utility.h"

#include "DeliveryTruck.h"

DeliveryTruck::DeliveryTruck() : GameObject("DeliveryTruck"), m_dir(2)
{
}

DeliveryTruck::~DeliveryTruck()
{
}

void DeliveryTruck::Init()
{

}

void DeliveryTruck::Update(double dt)
{

}

void DeliveryTruck::SetSprite()
{
    if (this->m_mesh->textureID > 0)
    {
        float texCountWidth = m_dir;
        float texWidth = 1.f / 4;
        
        TexCoord texCoords[4] = {
            TexCoord(texWidth * texCountWidth, 0),
            TexCoord(texWidth * (texCountWidth + 1), 0),
            TexCoord(texWidth * (texCountWidth + 1), 0.99f),
            TexCoord(texWidth * texCountWidth, 0.99f)
        };
        
        glBindBuffer(GL_ARRAY_BUFFER, m_mesh->vertexBuffer);
        for (unsigned i = 0; i < 4; ++i) {
            glBufferSubData(GL_ARRAY_BUFFER, (sizeof(Vertex)-sizeof(TexCoord)) + (i * sizeof(Vertex)), sizeof(TexCoord), &texCoords[i]);
        }
    }
}

void DeliveryTruck::SetDirection(int dir)
{
    this->m_dir = dir;
    this->SetSprite();
}