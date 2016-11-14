#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <string>
#include <vector>
#include "Vector3.h"
#include "../Mesh.h"

class GameObject
{
protected:
    std::string m_name;
    Vector3 m_pos;
    bool b_active;
    Mesh* m_mesh;

    bool b_isEntity;

    GameObject(std::string name, bool isEntity = false);

public:
    static std::vector<GameObject*> m_goList;

    virtual ~GameObject();

    virtual void Init() = 0;
    virtual void Update(double dt) = 0;      // doing things for its state (e.g. walking)

    std::string GetName();
    Vector3 GetPos();
    bool IsActive();
    Mesh* GetMesh();

    bool IsEntity();

    void SetPos(const Vector3& pos);
    void SetMesh(Mesh* mesh);
    void SetActive();
    void SetInactive();
};

#endif