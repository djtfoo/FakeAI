#ifndef SHAREDDATA_H
#define SHAREDDATA_H

#include "MeshList.h"
#include "GridMap.h"
#include "GameObject/GameObject.h"
#include "GameObject/OrnamentBuildingSystem.h"
#include "MessageBoard/MessageBoard.h"
#include <vector>

class SharedData
{
    SharedData();
    ~SharedData();

public:
    static SharedData* GetInstance()
    {
        static SharedData sharedData;
        return &sharedData;
    }

    MeshList* m_meshList;
    GridMap* m_gridMap;   // map that stores number of grids and collision information
    std::vector<GameObject*> m_goList;
    OrnamentBuildingSystem* m_ornamentSystem;
    MessageBoard* m_messageBoard;

    void Init();
    void Exit();

    void AddGameObject(GameObject* go, Mesh* mesh, int row, int col, bool wantNewMesh = false);    // add a GameObject to the scene, based on which grid u want it on
    void AddGameObject(GameObject* go, Mesh* mesh, const Vector3& pos, bool wantNewMesh = false);    // add a GameObject to the scene, based on the Vec3 pos u want it on
    void DeleteGameObject(GameObject* go);      // delete a GameObject from m_goList
};

#endif