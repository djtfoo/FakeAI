#include "MeshList.h"
#include "MeshBuilder.h"
#include "LoadTGA.h"

MeshList::MeshList()
{
}

MeshList::~MeshList()
{
}

void MeshList::Init()
{
    // Initialise Mesh List
    for (int i = 0; i < NUM_GEOMETRY; ++i)
    {
        meshList[i] = NULL;
    }

    // Machine
    meshList[GEO_MACHINE] = MeshBuilder::GenerateQuad("machine", Color(1, 0, 0));

    // Worker
    meshList[GEO_WORKER] = MeshBuilder::GenerateQuad("worker", Color(0, 0, 1));

    // Maintenance Man
    meshList[GEO_MAINTENANCEMAN] = MeshBuilder::GenerateQuad("maintenance man", Color(0, 1, 1));

    // Robot
    meshList[GEO_ROBOT] = MeshBuilder::GenerateQuad("robot", Color(0.5f, 0.5f, 0.5f));

    // Scrap Man
    meshList[GEO_SCRAPMAN] = MeshBuilder::GenerateQuad("scrap man", Color(0.3f, 0.3f, 0.3f));

    // Delivery Man
    meshList[GEO_DELIVERYMAN] = MeshBuilder::GenerateQuad("delivery man", Color(1, 0.5f, 0.5f));

    // Game Objects
    meshList[GEO_DELIVERYTRUCK];
    meshList[GEO_CONVEYORBELT] = MeshBuilder::GenerateQuad("conveyor belt", Color(0.3f, 0.5f, 0.5f));
    meshList[GEO_TOILET];
    meshList[GEO_WORKSTATION] = MeshBuilder::GenerateQuad("workstation", Color(0.5f, 0.2f, 0.07f));

    meshList[GEO_SCRAP_PIECE];
    meshList[GEO_SCRAP_STACK];

    meshList[GEO_ROBOT_HEAD] = MeshBuilder::GenerateQuad("robot head", Color(0.3f, 0.5f, 0.5f), 0.75);
    meshList[GEO_ROBOT_BODY] = MeshBuilder::GenerateQuad("robot body", Color(0.3f, 0.5f, 0.5f), 0.75);
    meshList[GEO_ROBOT_LIMBS] = MeshBuilder::GenerateQuad("robot limbs", Color(0.3f, 0.5f, 0.5f), 0.75);
    meshList[GEO_ROBOT_CHIP] = MeshBuilder::GenerateQuad("robot chip", Color(0.3f, 0.5f, 0.5f), 0.75);
    meshList[GEO_ROBOT_STAGE1] = MeshBuilder::GenerateQuad("robot stage1", Color(0.5f, 0.5f, 0.5f), 1);
    meshList[GEO_ROBOT_STAGE2];
    meshList[GEO_ROBOT_STAGE3];

    meshList[GEO_PYRAMID_PART];
    meshList[GEO_PYRAMID_STACK];
    meshList[GEO_PYRAMID_STAGE1];
    meshList[GEO_PYRAMID_STAGE2];
    meshList[GEO_PYRAMID_STAGE3];

    meshList[GEO_TEXT] = MeshBuilder::GenerateText("text", 16, 16);
    meshList[GEO_TEXT]->textureID = LoadTGA("Image//Fonts/Font_FFF Forward.tga");
    meshList[GEO_TEXT]->LoadFontData("Image//Fonts/FontData_FFF Forward.csv");
    meshList[GEO_TEXT]->material.kAmbient.Set(1, 0, 0);
}

void MeshList::Exit()
{
    for (int i = 0; i < NUM_GEOMETRY; ++i)
    {
        if (meshList[i])
            delete meshList[i];
    }
}

Mesh* MeshList::GetMesh(GEOMETRY_TYPE geo_type)
{
    return meshList[geo_type];
}