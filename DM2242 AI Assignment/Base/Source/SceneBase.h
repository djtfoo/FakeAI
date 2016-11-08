#ifndef SCENE_BASE_H
#define SCENE_BASE_H

#include "Scene.h"
#include "Mtx44.h"
#include "Camera.h"
#include "Mesh.h"
#include "MatrixStack.h"
#include "Light.h"
#include "GameObject/GameObject.h"
#include <vector>

class SceneBase : public Scene
{
	enum UNIFORM_TYPE
	{
		U_MVP = 0,
		U_MODELVIEW,
		U_MODELVIEW_INVERSE_TRANSPOSE,
		U_MATERIAL_AMBIENT,
		U_MATERIAL_DIFFUSE,
		U_MATERIAL_SPECULAR,
		U_MATERIAL_SHININESS,
		U_LIGHTENABLED,
		U_NUMLIGHTS,
		U_LIGHT0_TYPE,
		U_LIGHT0_POSITION,
		U_LIGHT0_COLOR,
		U_LIGHT0_POWER,
		U_LIGHT0_KC,
		U_LIGHT0_KL,
		U_LIGHT0_KQ,
		U_LIGHT0_SPOTDIRECTION,
		U_LIGHT0_COSCUTOFF,
		U_LIGHT0_COSINNER,
		U_LIGHT0_EXPONENT,
		U_COLOR_TEXTURE_ENABLED,
		U_COLOR_TEXTURE,
		U_TEXT_ENABLED,
		U_TEXT_COLOR,
		U_TOTAL,
	};
public:
	enum GEOMETRY_TYPE
	{
        // Entities
        GEO_MACHINE,
        GEO_WORKER,
        GEO_MAINTENANCEMAN,
        GEO_ROBOT,
        GEO_SCRAPMAN,
        GEO_DELIVERYMAN,

        // Game Objects
        GEO_DELIVERYTRUCK,
        GEO_CONVEYORBELT,
        GEO_TOILET,
        GEO_WORKSTATION,

        GEO_SCRAP_PIECE,
        GEO_SCRAP_STACK,

        GEO_ROBOT_HEAD,
        GEO_ROBOT_BODY,
        GEO_ROBOT_LIMBS,
        GEO_ROBOT_CHIP,
        GEO_ROBOT_STAGE1,
        GEO_ROBOT_STAGE2,
        GEO_ROBOT_STAGE3,

        GEO_PYRAMID_PART,
        GEO_PYRAMID_STACK,
        GEO_PYRAMID_STAGE1,
        GEO_PYRAMID_STAGE2,
        GEO_PYRAMID_STAGE3,

        // Generic
		GEO_TEXT,
		NUM_GEOMETRY,
	};
public:
	SceneBase();
	~SceneBase();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();

	void RenderText(Mesh* mesh, std::string text, Color color);
	void RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y);
	void RenderMesh(Mesh *mesh, bool enableLight);
	void RenderGO(GameObject* go);

	GameObject* FetchGO();
protected:
	unsigned m_vertexArrayID;
	Mesh* meshList[NUM_GEOMETRY];
	unsigned m_programID;
	unsigned m_parameters[U_TOTAL];

	Camera camera;

	MS modelStack;
	MS viewStack;
	MS projectionStack;

	Light lights[1];

	bool bLightEnabled;

	float fps;
};

#endif