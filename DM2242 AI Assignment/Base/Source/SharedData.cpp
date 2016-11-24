#include "SharedData.h"

SharedData::SharedData()
{
}

SharedData::~SharedData()
{
}

void SharedData::Init()
{
    m_meshList = new MeshList();
    m_meshList->Init();

    m_gridMap = new GridMap(15, 15);
}

void SharedData::Exit()
{
    if (m_meshList)
    {
        m_meshList->Exit();
        delete m_meshList;
        m_meshList = NULL;
    }

    if (m_gridMap)
    {
        m_gridMap->Exit();
        delete m_gridMap;
        m_gridMap = NULL;
    }

    //Cleanup GameObjects
    while (m_goList.size() > 0)
    {
        GameObject *go = m_goList.back();
        delete go;
        m_goList.pop_back();
    }
}

void SharedData::AddGameObject(GameObject* go, Mesh* mesh, int col, int row)
{
    // add to m_goList
    go->Init();
    go->SetPos(Vector3(col * 1.f, row * 1.f, 0));
    go->SetMesh(mesh);
    m_goList.push_back(go);

    // add collision to gridmap
    m_gridMap->m_collisionGrid[row][col] = true;
}

void SharedData::AddGameObject(GameObject* go, Mesh* mesh, const Vector3& pos)
{
    // add to m_goList
    go->Init();
    go->SetPos(pos);
    go->SetMesh(mesh);
    m_goList.push_back(go);

    // add collision to gridmap
    int row = (int)(pos.y) / m_gridMap->GetRows();
    int col = (int)(pos.x) / m_gridMap->GetColumns();
    m_gridMap->m_collisionGrid[row][col] = true;
}

void SharedData::DeleteGameObject(GameObject* go)
{

}