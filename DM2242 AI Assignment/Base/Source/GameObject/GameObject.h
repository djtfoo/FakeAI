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
    void SetMesh(Mesh* mesh);
    void SetActive();
    void SetInactive();

    virtual void SetSprite();   // edit the mesh TexCoords
    // if a GameObject doesn't have different sprites, don't need define, can use GameObject's default
};

#endif