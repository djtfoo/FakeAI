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

    //================
    // Create entities
    //================

    // Machine
    Machine* machine = new Machine();
    machine->SetPartToCreate(Machine::BODY);
    machine->SetPos(Vector3(2.5f, 16.5f, 0));
    machine->SetMesh(meshList[GEO_MACHINE]);
    m_goList.push_back(machine);

    machine = new Machine();
    machine->SetPartToCreate(Machine::HEAD);
    machine->SetPos(Vector3(10.5f, 17.5f, 0));
    machine->SetMesh(meshList[GEO_MACHINE]);
    m_goList.push_back(machine);

    machine = new Machine();
    machine->SetPartToCreate(Machine::LIMB);
    machine->SetPos(Vector3(10.5f, 10.5f, 0));
    machine->SetMesh(meshList[GEO_MACHINE]);
    m_goList.push_back(machine);

    machine = new Machine();
    machine->SetPartToCreate(Machine::MICROCHIP);
    machine->SetPos(Vector3(3.5f, 12.5f, 0));
    machine->SetMesh(meshList[GEO_MACHINE]);
    m_goList.push_back(machine);

    // Worker
    Worker* worker = new Worker();
    worker->SetPos(Vector3(7.5f, 17.5f, 0));
    worker->SetMesh(meshList[GEO_WORKER]);
    m_goList.push_back(worker);

    worker = new Worker();
    worker->SetPos(Vector3(11.5f, 13.5f, 0));
    worker->SetMesh(meshList[GEO_WORKER]);
    m_goList.push_back(worker);

    worker = new Worker();
    worker->SetPos(Vector3(6.5f, 10.5f, 0));
    worker->SetMesh(meshList[GEO_WORKER]);
    m_goList.push_back(worker);

    worker = new Worker();
    worker->SetPos(Vector3(2.5f, 8.5f, 0));
    worker->SetMesh(meshList[GEO_WORKER]);
    m_goList.push_back(worker);

    MaintenanceMan* maintenanceman = new MaintenanceMan();
    maintenanceman->SetPos(Vector3(15.5f, 7.5f, 0));
    maintenanceman->SetMesh(meshList[GEO_MAINTENANCEMAN]);
    m_goList.push_back(maintenanceman);

    ScrapMan* scrapman = new ScrapMan();
    scrapman->SetPos(Vector3(14.5f, 2.5f, 0));
    scrapman->SetMesh(meshList[GEO_SCRAPMAN]);
    m_goList.push_back(scrapman);
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
    
    for (std::vector<GameObject*>::iterator it = m_goList.begin(); it != m_goList.end(); ++it)
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
	projection.SetToOrtho(0, 20, 0, 20, -10, 10);
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

	for (std::vector<GameObject*>::iterator it = m_goList.begin(); it != m_goList.end(); ++it)
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
	//while (m_goList.size() > 0)
	//{
	//	GameObject *go = m_goList.back();
	//	delete go;
	//	m_goList.pop_back();
	//}
}
