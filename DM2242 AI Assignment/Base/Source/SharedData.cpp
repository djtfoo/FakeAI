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

    m_ornamentSystem = new OrnamentBuildingSystem();
    //m_ornamentSystem->Init();

    m_messageBoard = new MessageBoard();

    // randomise the starting day (and time)
    Time::DAY startingDay = static_cast<Time::DAY>(Math::RandIntMinMax(0, 4));
    m_clock = new Clock(Time(startingDay, 32400));
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

    if (m_ornamentSystem)
    {
        m_ornamentSystem->Exit();
        delete m_ornamentSystem;
        m_ornamentSystem = NULL;
    }

    if (m_messageBoard)
    {
        m_messageBoard->Exit();
        delete m_messageBoard;
        m_messageBoard = NULL;
    }

    if (m_clock)
    {
        delete m_clock;
        m_clock = NULL;
    }

    //Cleanup GameObjects
    while (m_goList.size() > 0)
    {
        GameObject *go = m_goList.back();
        delete go;
        m_goList.pop_back();
    }
}

void SharedData::AddGameObject(GameObject* go, Mesh* mesh, int col, int row, bool wantNewMesh)
{
    // add to m_goList
    go->Init();
    go->SetPos(Vector3(col * 1.f, row * 1.f, 0));
    go->SetMesh(mesh, wantNewMesh);
    m_goList.push_back(go);

    // add collision to gridmap
    m_gridMap->m_collisionGrid[row][col] = true;

    if (go->GetName() == "Building Block Stack")
    {
        SharedData::GetInstance()->m_ornamentSystem->SetBuildingBlockStack(dynamic_cast<BuildingBlockStack*>(go));
    }
    else if (go->GetName() == "Ornament")
    {
        SharedData::GetInstance()->m_ornamentSystem->SetOrnament(dynamic_cast<Ornament*>(go));
    }
}

void SharedData::AddGameObject(GameObject* go, Mesh* mesh, const Vector3& pos, bool wantNewMesh)
{
    // add to m_goList
    go->Init();
    go->SetPos(pos);
    go->SetMesh(mesh, wantNewMesh);
    m_goList.push_back(go);

    // add collision to gridmap
    int row = (int)(pos.y) / m_gridMap->GetRows();
    int col = (int)(pos.x) / m_gridMap->GetColumns();
    m_gridMap->m_collisionGrid[row][col] = true;
}

void SharedData::DeleteGameObject(GameObject* go)
{

}