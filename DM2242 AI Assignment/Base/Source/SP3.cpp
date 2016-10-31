#include "SP3.h"
#include "GL\glew.h"
#include "Application.h"
#include <sstream>
#include "MeshBuilder.h"
#include "LoadTGA.h"

#include <fstream>
#include <sstream>

SP3::SP3()
{
}

SP3::~SP3()
{
}

void SP3::Init()
{
	SceneBase::Init();

	//Calculating aspect ratio
	m_worldHeight = 100.f;
	m_worldWidth = m_worldHeight * (float)Application::GetWindowWidth() / Application::GetWindowHeight();

	//Physics code here
	m_speed = 1.f;

	Math::InitRNG();

	m_objectCount = 0;

	// ----------------- Code needs to be fixed ----------------- // 
	//m_ghost = new GameObject();
	// ---------------------------------------------------------- // 

	// ----------------- Example of Spawning Objects ------------ // 
	// SpawnGameObject(OBJECT_TYPE (Eg. Environment, Projectile etc.), GAMEOBJECT_TYPE (Eg. GO_BALL, etc.), Position, Scale, Collidable, Visible)

	// ---------------------------------------------------------- // 
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

void SP3::Update(double dt)
{
	SceneBase::Update(dt);
}

//void SP3::RenderGO(GameObject *go)
//{
//	modelStack.PushMatrix();
//	modelStack.Translate(go->GetPosition().x, go->GetPosition().y, go->GetPosition().z);
//	modelStack.Rotate(0, 0, 1, 0);
//	modelStack.Scale(go->GetScale().x, go->GetScale().y, go->GetScale().z);
//	RenderMesh(go->GetMesh(), false);
//	modelStack.PopMatrix();
//}

void SP3::Render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//Calculating aspect ratio
	m_worldHeight = 100.f;
	m_worldWidth = m_worldHeight * (float)Application::GetWindowWidth() / Application::GetWindowHeight();

	// Projection matrix : Orthographic Projection
	Mtx44 projection;
	projection.SetToOrtho(0, m_worldWidth, 0, m_worldHeight, -10, 10);
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

	RenderMesh(meshList[GEO_AXES], false);

	//for (std::vector<GameObject *>::iterator it = m_goList.begin(); it != m_goList.end(); ++it)
	//{
	//	GameObject *go = (GameObject *)*it;
	//	if (go->GetActive() && go->GetVisible())
	//	{
	//		RenderGO(go);
	//	}
	//}

}

void SP3::Exit()
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
