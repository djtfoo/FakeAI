#include "GL\glew.h"
#include "../Utility.h"

#include "DeliveryTruck.h"

DeliveryTruck::DeliveryTruck() : GameObject("DeliveryTruck")
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
        //float texCountWidth = STAGES_TOTAL - 1 - m_stage;
        //float texWidth = 1.f / STAGES_TOTAL;
        //
        //TexCoord texCoords[4] = {
        //    TexCoord(0, texHeight * texCountHeight),
        //    TexCoord(1, texHeight * texCountHeight),
        //    TexCoord(1, texHeight * (texCountHeight + 1) - 0.01f),
        //    TexCoord(0, texHeight * (texCountHeight + 1) - 0.01f)
        //};
        //
        //glBindBuffer(GL_ARRAY_BUFFER, m_mesh->vertexBuffer);
        //for (unsigned i = 0; i < 4; ++i) {
        //    glBufferSubData(GL_ARRAY_BUFFER, (sizeof(Vertex)-sizeof(TexCoord)) + (i * sizeof(Vertex)), sizeof(TexCoord), &texCoords[i]);
        //}
    }
}