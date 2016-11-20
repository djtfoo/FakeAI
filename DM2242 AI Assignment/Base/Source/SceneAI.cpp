#include "SceneAI.h"
#include "GL\glew.h"
#include "Application.h"
#include <sstream>
#include "SharedData.h"

#include <fstream>
#include <sstream>

#include "GameObject/Entity/Machine.h"
#include "GameObject/Entity/Worker.h"
#include "GameObject/Entity/MaintenanceMan.h"
#include "GameObject/Entity/Robot.h"
#include "GameObject/Entity/DeliveryMan.h"
#include "GameObject/Entity/ScrapMan.h"

#include "GameObject/Toilet.h"

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

    // Initialise shared variables
    SharedData::GetInstance()->Init();

    //===================
    // Create GameObjects
    //===================

    // Conveyor Belt
    ConveyorBelt* conveyor = new ConveyorBelt();
    conveyor->SetPos(Vector3(3, 16, 0));
    conveyor->Init();
    conveyor->SetMesh(SharedData::GetInstance()->m_meshList->GetMesh(GEO_CONVEYORBELT));
    conveyor->AddCheckpoint(Vector3(7, 16, 0));
    conveyor->AddCheckpoint(Vector3(10, 16, 0));
    conveyor->AddCheckpoint(Vector3(10, 13, 0));
    conveyor->AddCheckpoint(Vector3(10, 11, 0));
    conveyor->AddCheckpoint(Vector3(6, 11, 0));
    conveyor->AddCheckpoint(Vector3(3, 11, 0));
    conveyor->AddCheckpoint(Vector3(3, 8, 0));
    SharedData::GetInstance()->m_goList.push_back(conveyor);

    // Toilet
    SharedData::GetInstance()->AddGameObject(new Toilet(), SharedData::GetInstance()->m_meshList->GetMesh(GEO_TOILET), 18, 18);

    // Machine
    debugMachine = new Machine();
    debugMachine->Init();
    debugMachine->SetPartToCreate(RobotPart::BODY);
    debugMachine->SetPos(Vector3(2.f, 16.f, 0));
    debugMachine->SetMesh(SharedData::GetInstance()->m_meshList->GetMesh(GEO_MACHINE));
    debugMachine->SetSpawnLocation(conveyor->GetCheckpoint(0));
    SharedData::GetInstance()->m_goList.push_back(debugMachine);

    SharedData::GetInstance()->m_gridMap->m_collisionGrid[16][2] = true;

    Machine* machine = new Machine();
    machine->Init();
    machine->SetPartToCreate(RobotPart::HEAD);
    machine->SetPos(Vector3(10.f, 17.f, 0));
    machine->SetMesh(SharedData::GetInstance()->m_meshList->GetMesh(GEO_MACHINE));
    machine->SetSpawnLocation(conveyor->GetCheckpoint(2));
    SharedData::GetInstance()->m_goList.push_back(machine);
    SharedData::GetInstance()->m_gridMap->m_collisionGrid[17][10] = true;

    machine = new Machine();
    machine->Init();
    machine->SetPartToCreate(RobotPart::LIMB);
    machine->SetPos(Vector3(10.f, 10.f, 0));
    machine->SetMesh(SharedData::GetInstance()->m_meshList->GetMesh(GEO_MACHINE));
    machine->SetSpawnLocation(conveyor->GetCheckpoint(4));
    SharedData::GetInstance()->m_goList.push_back(machine);
    SharedData::GetInstance()->m_gridMap->m_collisionGrid[10][10] = true;

    machine = new Machine();
    machine->Init();
    machine->SetPartToCreate(RobotPart::MICROCHIP);
    machine->SetPos(Vector3(3.f, 12.f, 0));
    machine->SetMesh(SharedData::GetInstance()->m_meshList->GetMesh(GEO_MACHINE));
    machine->SetSpawnLocation(conveyor->GetCheckpoint(6));
    SharedData::GetInstance()->m_goList.push_back(machine);
    SharedData::GetInstance()->m_gridMap->m_collisionGrid[12][3] = true;

    // Worker + Assosiated Workstation ( 1 )
    Workstation* tempStation = new Workstation();
    tempStation->Init();
    tempStation->SetActive();
    tempStation->SetMesh(SharedData::GetInstance()->m_meshList->GetMesh(GEO_WORKSTATION));
    tempStation->SetPos(Vector3(6, 17, 0));
    tempStation->SetTypeStored(RobotPart::BODY);
    SharedData::GetInstance()->m_goList.push_back(tempStation);

    SharedData::GetInstance()->m_gridMap->m_collisionGrid[17][6] = true;

    Worker* tempWorker = new Worker();
    tempWorker->Init();
    tempWorker->SetActive();
    tempWorker->SetMesh(SharedData::GetInstance()->m_meshList->GetMesh(GEO_WORKER));
    tempWorker->SetPos(Vector3(7, 17, 0));
    tempWorker->SetWorkstation(tempStation);
    SharedData::GetInstance()->m_goList.push_back(tempWorker);

    // Worker + Assosiated Workstation ( 2 )
    tempStation = new Workstation();
    tempStation->Init();
    tempStation->SetActive();
    tempStation->SetMesh(SharedData::GetInstance()->m_meshList->GetMesh(GEO_WORKSTATION));
    tempStation->SetPos(Vector3(11, 14, 0));
    tempStation->SetTypeStored(RobotPart::HEAD);
    SharedData::GetInstance()->m_goList.push_back(tempStation);

    SharedData::GetInstance()->m_gridMap->m_collisionGrid[17][6] = true;

    tempWorker = new Worker();
    tempWorker->Init();
    tempWorker->SetActive();
    tempWorker->SetMesh(SharedData::GetInstance()->m_meshList->GetMesh(GEO_WORKER));
    tempWorker->SetPos(Vector3(11, 13, 0));
    tempWorker->SetWorkstation(tempStation);
    SharedData::GetInstance()->m_goList.push_back(tempWorker);

    // Worker + Assosiated Workstation ( 3 )
    tempStation = new Workstation();
    tempStation->Init();
    tempStation->SetActive();
    tempStation->SetMesh(SharedData::GetInstance()->m_meshList->GetMesh(GEO_WORKSTATION));
    tempStation->SetPos(Vector3(7, 10, 0));
    tempStation->SetTypeStored(RobotPart::LIMB);
    SharedData::GetInstance()->m_goList.push_back(tempStation);

    SharedData::GetInstance()->m_gridMap->m_collisionGrid[10][7] = true;

    tempWorker = new Worker();
    tempWorker->Init();
    tempWorker->SetActive();
    tempWorker->SetMesh(SharedData::GetInstance()->m_meshList->GetMesh(GEO_WORKER));
    tempWorker->SetPos(Vector3(6, 10, 0));
    tempWorker->SetWorkstation(tempStation);
    SharedData::GetInstance()->m_goList.push_back(tempWorker);

    // Worker + Assosiated Workstation ( 4 )
    tempStation = new Workstation();
    tempStation->Init();
    tempStation->SetActive();
    tempStation->SetMesh(SharedData::GetInstance()->m_meshList->GetMesh(GEO_WORKSTATION));
    tempStation->SetPos(Vector3(2, 9, 0));
    tempStation->SetTypeStored(RobotPart::MICROCHIP);
    SharedData::GetInstance()->m_goList.push_back(tempStation);

    SharedData::GetInstance()->m_gridMap->m_collisionGrid[9][2] = true;

    tempWorker = new Worker();
    tempWorker->Init();
    tempWorker->SetActive();
    tempWorker->SetMesh(SharedData::GetInstance()->m_meshList->GetMesh(GEO_WORKER));
    tempWorker->SetPos(Vector3(2, 8, 0));
    tempWorker->SetWorkstation(tempStation);
    SharedData::GetInstance()->m_goList.push_back(tempWorker);

    // Maintenance Man
    SharedData::GetInstance()->AddGameObject(new MaintenanceMan(), SharedData::GetInstance()->m_meshList->GetMesh(GEO_MAINTENANCEMAN), 15, 7);

    // Scrap Man
    SharedData::GetInstance()->AddGameObject(new ScrapMan(), SharedData::GetInstance()->m_meshList->GetMesh(GEO_SCRAPMAN), 14, 2);

    // pathfinder test
    pathfinderTest.ReceiveCurrentPos(Vector3(rand() % 20, rand() % 20, 0));
    //pathfinderTest.ReceiveCurrentPos(Vector3(2, 17, 0));

    pathfinderTest.ReceiveDestination(Vector3(18, 18, 0));
    pathfinderTest.FindPathGreedyBestFirst();
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

    for (int i = 0; i < SharedData::GetInstance()->m_goList.size(); ++i)
    {
        GameObject* go = SharedData::GetInstance()->m_goList[i];
        go->Update(dt);

        if (go->IsEntity())
        {
            Entity* entity = dynamic_cast<Entity*>(go);
            entity->RunFSM(dt);
        }
    }

    // DEBUG 

    if (Application::IsKeyPressed('1'))
        debugMachine->SetIsBroken(false);

    if (Application::IsKeyPressed('2'))
        debugMachine->SetIsEmpty(false);
}

void SceneAI::RenderGO(GameObject *go)
{
	modelStack.PushMatrix();
	modelStack.Translate(go->GetPos().x, go->GetPos().y, go->GetPos().z);
    modelStack.Scale(go->GetScale().x, go->GetScale().y, go->GetScale().z);
	RenderMesh(go->GetMesh(), false);
	modelStack.PopMatrix();
}

void SceneAI::Render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Projection matrix : Orthographic Projection
	Mtx44 projection;
    projection.SetToOrtho(0.5, SharedData::GetInstance()->m_gridMap->GetRows() - 0.5, -0.5, SharedData::GetInstance()->m_gridMap->GetColumns() - 0.5, -10, 10);
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

    for (std::vector<GameObject*>::iterator it = SharedData::GetInstance()->m_goList.begin(); it != SharedData::GetInstance()->m_goList.end(); ++it)
    {
        GameObject *go = (GameObject *)*it;
        if (go->IsActive())
        {
            // Special Case for conveyor belt
            if (go->GetName() == "ConveyorBelt")
            {
                ConveyorBelt* belt = dynamic_cast<ConveyorBelt*>(go);

                for (int idx = 0; idx < belt->m_Checkpoints.size() - 1; ++idx)
                {
                    int idx2 = idx + 1;
                    Vector3 spawn = belt->m_Checkpoints[idx];
                    while (spawn != belt->m_Checkpoints[idx2])
                    {
                        if (spawn.x < belt->m_Checkpoints[idx2].x)
                            spawn.x += 1;
                        if (spawn.x > belt->m_Checkpoints[idx2].x)
                            spawn.x -= 1;
                        if (spawn.y < belt->m_Checkpoints[idx2].y)
                            spawn.y += 1;
                        if (spawn.y > belt->m_Checkpoints[idx2].y)
                            spawn.y -= 1;

                        modelStack.PushMatrix();
                        modelStack.Translate(spawn.x, spawn.y, -1);
                        modelStack.Scale(go->GetScale().x, go->GetScale().y, go->GetScale().z);
                        //RenderMesh(SharedData::GetInstance()->m_meshList->GetMesh(GEO_CONVEYORBELT), false);
                        modelStack.PopMatrix();

                        // This should be moved somewhere else, Init maybe
                        SharedData::GetInstance()->m_gridMap->m_collisionGrid[(int)spawn.y][(int)spawn.x] = true;
                    }
                    
                }
            }

            RenderGO(go);
        }
    }

    
    modelStack.PushMatrix();
    modelStack.Translate(pathfinderTest.position.x, pathfinderTest.position.y, 0);
    RenderMesh(SharedData::GetInstance()->m_meshList->GetMesh(GEO_ROBOT_HEAD), false);
    modelStack.PopMatrix();

    for (std::vector<Node>::iterator it = pathfinderTest.foundPath.begin(); it != pathfinderTest.foundPath.end(); ++it)
    {
        modelStack.PushMatrix();
        modelStack.Translate((*it).col, (*it).row, 0);
        RenderMesh(SharedData::GetInstance()->m_meshList->GetMesh(GEO_PATHFINDING_NODE), false);
        modelStack.PopMatrix();
    }

    // DEBUG 
    switch (debugMachine->GetState())
    {
    case Machine::REST:
        std::cout << "Machine: REST             Timer: " << debugMachine->GetTimer() << std::endl;
        break;
    case Machine::PRODUCTION:
        std::cout << "Machine: PRODUCTION       Timer: " << debugMachine->GetTimer() << std::endl;
        break;
    case Machine::BROKEN:
        std::cout << "Machine: BROKEN           Timer: " << debugMachine->GetTimer() << std::endl;
        break;
    case Machine::WAITFORREFILL:
        std::cout << "Machine: WAITFORREFILL    Timer: " << debugMachine->GetTimer() << std::endl;
        break;
    }
    
}

void SceneAI::Exit()
{
	SceneBase::Exit();

    SharedData::GetInstance()->Exit();
}
