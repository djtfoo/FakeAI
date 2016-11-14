#ifndef SCENE_AI_H
#define SCENE_AI_H

#include "SceneBase.h"

#include "GridMap.h"
#include "GameObject/GameObject.h"

class SceneAI : public SceneBase
{

public:
	SceneAI();
	~SceneAI();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();

	void RenderGO(GameObject *go);

	//GameObject* FetchGameObject(OBJECT_TYPE ObjectType);
	//void SpawnGameObject(OBJECT_TYPE ObjectType, GAMEOBJECT_TYPE GoType, Vector3 Position, Vector3 Scale, bool Collidable, bool Visible);

protected:
    // Scene Management
    GridMap* m_gridMap;  // map that stores number of grids and collision information
    void AddGameObject(GameObject* go, Mesh* mesh, int row, int col);    // add a GameObject to the scene, based on which grid u want it on
    void AddGameObject(GameObject* go, Mesh* mesh, const Vector3& pos);    // add a GameObject to the scene, based on the Vec3 pos u want it on
    void DeleteGameObject(GameObject* go);      // delete a GameObject from m_goList
};

#endif