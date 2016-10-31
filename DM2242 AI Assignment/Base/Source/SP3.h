#ifndef SCENE_ASSIGNMENT2_H
#define SCENE_ASSIGNMENT2_H

#include <vector>
#include "SceneBase.h"

class SP3 : public SceneBase
{

public:
	SP3();
	~SP3();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();

	//void RenderGO(GameObject *go);

	//GameObject* FetchGameObject(OBJECT_TYPE ObjectType);
	//void SpawnGameObject(OBJECT_TYPE ObjectType, GAMEOBJECT_TYPE GoType, Vector3 Position, Vector3 Scale, bool Collidable, bool Visible);

protected:

	//std::vector<GameObject *> m_goList;

	float m_speed;
	float m_worldWidth;
	float m_worldHeight;
	//GameObject *m_ghost;
	int m_objectCount;
	float m_gravity;

};

#endif