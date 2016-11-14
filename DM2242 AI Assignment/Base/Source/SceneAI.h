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
    
};

#endif