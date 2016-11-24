#include "GL\glew.h"
#include "../Utility.h"

#include "Toilet.h"

Toilet::Toilet() : GameObject("Toilet"), b_occupied(false)
{
}

Toilet::~Toilet()
{
}

void Toilet::Init()
{
    //m_scale.Set();
}

void Toilet::Update(double dt)
{

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