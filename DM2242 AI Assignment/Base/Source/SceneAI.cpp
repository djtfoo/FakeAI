#include "SceneAI.h"
#include "GL\glew.h"
#include "Application.h"
#include <sstream>
#include "MeshBuilder.h"
#include "LoadTGA.h"

#include <fstream>
#include <sstream>

#include "GameObject/Entity/Machine.h"
#include "GameObject/Entity/Worker.h"
#include "GameObject/Entity/MaintenanceMan.h"
#include "GameObject/Entity/Robot.h"
#include "GameObject/Entity/DeliveryMan.h"
#include "GameObject/Entity/ScrapMan.h"

SceneAI::SceneAI()
{
}

SceneAI::~SceneAI()
{
}

void SceneAI::Init()
{
    SceneBase::Init();

    Math::InitRNG();

    // Initialise grid map
    m_gridMap = new GridMap(20, 20);

    //================
    // Create entities
    //================

    // Conveyor Belt
    ConveyorBelt* conveyor = new ConveyorBelt();
    conveyor->Init();
    conveyor->SetPos(Vector3(3, 16, 0));
    conveyor->SetMesh(meshList[GEO_CONVEYORBELT]);
    GameObject::m_goList.push_back(conveyor);

    // Machine
    Machine* machine = new Machine();
    machine->Init();
    machine->SetPartToCreate(RobotPart::BODY);
    machine->SetPos(Vector3(2.f, 16.f, 0));
    machine->SetMesh(meshList[GEO_MACHINE]);
    machine->SetSpawnLocation(conveyor);
    GameObject::m_goList.push_back(machine);


    //machine = new Machine();
    //machine->Init();
    //machine->SetPartToCreate(RobotPart::HEAD);
    //machine->SetPos(Vector3(10.f, 17.f, 0));
    //machine->SetMesh(meshList[GEO_MACHINE]);
    //GameObject::m_goList.push_back(machine);

    //machine = new Machine();
    //machine->Init();
    //machine->SetPartToCreate(RobotPart::LIMB);
    //machine->SetPos(Vector3(10.f, 10.f, 0));
    //machine->SetMesh(meshList[GEO_MACHINE]);
    //GameObject::m_goList.push_back(machine);

    //machine = new Machine();
    //machine->Init();
    //machine->SetPartToCreate(RobotPart::MICROCHIP);
    //machine->SetPos(Vector3(3.f, 12.f, 0));
    //machine->SetMesh(meshList[GEO_MACHINE]);
    //GameObject::m_goList.push_back(machine);

    // Worker
    AddGameObject(new Worker(), meshList[GEO_WORKER], 7, 17);
    AddGameObject(new Worker(), meshList[GEO_WORKER], 11, 13);
    AddGameObject(new Worker(), meshList[GEO_WORKER], 6, 10);
    AddGameObject(new Worker(), meshList[GEO_WORKER], 2, 8);

    // Maintenance Man
    AddGameObject(new MaintenanceMan(), meshList[GEO_MAINTENANCEMAN], 15, 7);

    // Scrap Man
    AddGameObject(new ScrapMan(), meshList[GEO_SCRAPMAN], 14, 2);
}

void SceneAI::AddGameObject(GameObject* go, Mesh* mesh, int row, int col)
{
    // add to m_goList
    go->SetPos(Vector3(row * 1.f, col * 1.f, 0));
    go->SetMesh(mesh);
    GameObject::m_goList.push_back(go);

    // add collision to gridmap
    m_gridMap->m_collisionGrid[row][col] = true;
}

void SceneAI::AddGameObject(GameObject* go, Mesh* mesh, const Vector3& pos)
{
    // add to m_goList
    go->SetPos(pos);
    go->SetMesh(mesh);
    GameObject::m_goList.push_back(go);

    // add collision to gridmap
    int row = (int)(pos.y) / m_gridMap->GetRows();
    int col = (int)(pos.x) / m_gridMap->GetColumns();
    m_gridMap->m_collisionGrid[row][col] = true;
}

void SceneAI::DeleteGameObject(GameObject* go)
{

}

//GameObject* SP3::FetchGameObject(OBJECT_TYPE ObjectType)
//{
//	GameObject* TempGameObject = NULL;
//
//	switch (ObjectType)
//	{
//		case OBJECT_TYPE::ENVIRONMENT:
//		{
//			TempGameObject = new Environment;
//			break;
//		}
//
//		case OBJECT_TYPE::PROJECTILE:
//		{
//			break;
//		}
//
//		case OBJECT_TYPE::PLAYER:
//		{		
//			break;
//		}
//
//		case OBJECT_TYPE::ENEMY:
//		{						
//			break;
//		}
//	}
//
//	return TempGameObject;
//}

//void SP3::SpawnGameObject(OBJECT_TYPE ObjectType, GAMEOBJECT_TYPE GoType, Vector3 Position, Vector3 Scale, bool Collidable, bool Visible)
//{
//	GameObject* go = FetchGameObject(ObjectType);
//
//	go->SetType(GoType);
//	go->SetPosition(Position);
//	go->SetScale(Scale);
//	go->SetCollidable(Collidable);
//	go->SetVisible(Visible);
//
//	switch (GoType)
//	{
//		case GO_BALL:
//		{
//			go->SetMesh(meshList[GO_BALL]);
//			break;
//		}
//	}
//
//	m_goList.push_back(go);
//}

void SceneAI::Update(double dt)
{
	SceneBase::Update(dt);
    
    for (std::vector<GameObject*>::iterator it = GameObject::m_goList.begin(); it != GameObject::m_goList.end(); ++it)
    {
        GameObject* go = dynamic_cast<GameObject*>(*it);
        go->Update(dt);

        if (go->IsEntity())
        {
            Entity* entity = dynamic_cast<Entity*>(go);
            entity->RunFSM(dt);
        }
    }
}

void SceneAI::RenderGO(GameObject *go)
{
	modelStack.PushMatrix();
	modelStack.Translate(go->GetPos().x, go->GetPos().y, go->GetPos().z);
	//modelStack.Scale(1, 1, 1);
	RenderMesh(go->GetMesh(), false);
	modelStack.PopMatrix();
}

void SceneAI::Render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Projection matrix : Orthographic Projection
	Mtx44 projection;
	projection.SetToOrtho(0.5, m_gridMap->GetRows() - 0.5, -0.5, m_gridMap->GetColumns() - 0.5, -10, 10);
	projectionStack.LoadMatrix(projection);

	// Camera matrix
	viewStack.LoadIdentity();
	viewStack.LookAt(
		camera.position.x, camera.position.y, camera.position.z,
		camera.target.x, camera.target.y, camera.target.z,
		camera.up.x, camera.up.y, camera.up.z
		);
	// Model matrix : an identity matrix (model will be at the origin)
	modelStack.LoadIdentity();

	for (std::vector<GameObject*>::iterator it = GameObject::m_goList.begin(); it != GameObject::m_goList.end(); ++it)
	{
		GameObject *go = (GameObject *)*it;
		if (go->IsActive())
		{
			RenderGO(go);
		}
	}

}

void SceneAI::Exit()
{
	SceneBase::Exit();
	//Cleanup GameObjects
	while (GameObject::m_goList.size() > 0)
	{
		GameObject *go = GameObject::m_goList.back();
		delete go;
		GameObject::m_goList.pop_back();
	}

    // Cleanup GridMap
    if (m_gridMap)
    {
        delete m_gridMap;
        m_gridMap = NULL;
    }
}
