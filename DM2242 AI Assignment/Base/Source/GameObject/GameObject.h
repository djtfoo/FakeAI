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
    Vector3 m_scale;
    bool b_active;
    Mesh* m_mesh;

    bool b_isEntity;

    GameObject(std::string name, bool isEntity = false);

public:
    virtual ~GameObject();

    virtual void Init() = 0;
    virtual void Update(double dt) = 0;      // doing things for its state (e.g. walking)

    std::string GetName();
    Vector3 GetPos();
    Vector3 GetScale();
    bool IsActive();
    Mesh* GetMesh();

    bool IsEntity();

    void SetPos(const Vector3& pos);
    void SetMesh(Mesh* mesh, bool wantNewMesh = false);
    void SetActive();
    void SetInactive();

    virtual void SetSprite();   // edit the mesh TexCoords
    // if a GameObject doesn't have different sprites, don't need define, can use GameObject's default

    // Wrapper function to assist in rounding off of velocity value
    float SmallRoundOff(float num)
    {
        if (num > Math::EPSILON)    // positive
        {
            if (num > 0.5f)
                return 1.f;

            else
                return 0.f;
        }
        else    // negative
        {
            if (num < -0.5f)
                return -1.f;

            else
                return 0.f;
        }
    }

    float RoundOff(float num)
    {
        float value = floor(num);
        float remainder = num - value;

        if (num > Math::EPSILON)    // positive
        {
            if (remainder > 0.5f)
                return 1.f + value;

            else
                return value;
        }
        else    // negative
        {
            if (remainder < -0.5f)
                return -1.f + value;

            else
                return value;
        }
    }
};

#endif