#include "GL\glew.h"
#include "../Utility.h"

#include "Ornament.h"

Ornament::Ornament() : GameObject("Ornament")
{
}

Ornament::~Ornament()
{
}

void Ornament::Init()
{
    m_partCount = 0;
    m_stage = STAGE_1;
}

void Ornament::Update(double dt)
{
    switch (m_stage)
    {
    case STAGE_1:
        if (m_partCount > 3)
        {
            m_stage = STAGE_2;
            SetSprite();
        }
        break;

    case STAGE_2:
        if (m_partCount > 6)
        {
            m_stage = STAGE_3;
            SetSprite();
        }
        break;

    case STAGE_3:
        if (m_partCount > 10)
        {
            m_stage = STAGE_COMPLETE;
            SetSprite();
        }
        break;
    }
}

void Ornament::SetSprite()
{
    if (this->m_mesh->textureID > 0)
    {
        float texCountHeight = STAGES_TOTAL - 1 - m_stage;
        float texHeight = 1.f / STAGES_TOTAL;

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

void Ornament::AddPart()
{
    ++m_partCount;
}

bool Ornament::isComplete()
{
    if (m_stage == STAGE_COMPLETE)
        return true;

    return false;
}