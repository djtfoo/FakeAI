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
    meshList[GEO_MACHINE] = MeshBuilder::GenerateQuad("machine", Color(1, 0.5f, 0));
    meshList[GEO_MACHINE]->textureID = LoadTGA("Image/Entity_Machine.tga");

    //meshList[GEO_MACHINE_PRODUCTION] = MeshBuilder::GenerateQuad("machine production", Color(1, 1, 0));
    //meshList[GEO_MACHINE_BROKEN] = MeshBuilder::GenerateQuad("machine broken", Color(1, 0.2f, 0.2f));
    //meshList[GEO_MACHINE_WAITFORREFILL] = MeshBuilder::GenerateQuad("machine wait", Color(1, 0.5f, 0.5f));

    // Worker
    meshList[GEO_WORKER] = MeshBuilder::GenerateQuad("worker", Color(0, 0, 1));
    meshList[GEO_WORKER]->textureID = LoadTGA("Image/Entity_Worker.tga");
    //meshList[GEO_WORKER_WORKING] = MeshBuilder::GenerateQuad("worker working", Color(0.5, 0.5, 1));

    // Maintenance Man
    meshList[GEO_MAINTENANCEMAN] = MeshBuilder::GenerateQuad("maintenance man", Color(0, 1, 1));
    meshList[GEO_MAINTENANCEMAN]->textureID = LoadTGA("Image/Entity_MaintenanceMan.tga");

    // Robot
    meshList[GEO_ROBOT] = MeshBuilder::GenerateQuad("robot", Color(0.5f, 0.5f, 0.5f));
    meshList[GEO_ROBOT]->textureID = LoadTGA("Image/Entity_Robot.tga");

    // Scrap Man
    meshList[GEO_SCRAPMAN] = MeshBuilder::GenerateQuad("scrap man", Color(0.3f, 0.3f, 0.3f));
    meshList[GEO_SCRAPMAN]->textureID = LoadTGA("Image/Entity_ScrapMan.tga");

    // Delivery Man
    meshList[GEO_DELIVERYMAN] = MeshBuilder::GenerateQuad("delivery man", Color(1, 0.5f, 0.5f));
    meshList[GEO_DELIVERYMAN]->textureID = LoadTGA("Image/Entity_DeliveryMan.tga");

    // Game Objects
    meshList[GEO_DELIVERYTRUCK] = MeshBuilder::GenerateQuad("delivery truck", Color(1, 0.5f, 0.5f));
    meshList[GEO_DELIVERYTRUCK]->textureID = LoadTGA("Image/GO_DeliveryTruck.tga");

    meshList[GEO_TOILET] = MeshBuilder::GenerateQuad("toilet", Color(0.8f, 0.95f, 0.9f));
    meshList[GEO_TOILET]->textureID = LoadTGA("Image//GO_Toilet.tga");

    meshList[GEO_WORKSTATION] = MeshBuilder::GenerateQuad("workstation", Color(0.5f, 0.2f, 0.07f));
    meshList[GEO_WORKSTATION]->textureID = LoadTGA("Image/GO_Workstation.tga");

    meshList[GEO_SCRAP_PILE] = MeshBuilder::GenerateQuad("scrap pile", Color(0.5f, 0.2f, 0.07f));
    meshList[GEO_SCRAP_PILE]->textureID = LoadTGA("Image/GO_ScrapPile.tga");

    meshList[GEO_ROBOT_PART_PACKAGE] = MeshBuilder::GenerateQuad("robot part", Color(0.3f, 0.5f, 0.5f));
    meshList[GEO_ROBOT_PART_PACKAGE]->textureID = LoadTGA("Image/GO_RobotPartPackage.tga");

    meshList[GEO_BUILDINGBLOCK_STACK] = MeshBuilder::GenerateQuad("building blocks", Color(0.3f, 0.5f, 0.5f));
    meshList[GEO_BUILDINGBLOCK_STACK]->textureID = LoadTGA("Image/GO_BuildingBlocks.tga");

    meshList[GEO_ORNAMENT] = MeshBuilder::GenerateQuad("ornament", Color(0.3f, 0.5f, 0.5f));
    meshList[GEO_ORNAMENT]->textureID = LoadTGA("Image/GO_Ornament.tga");

    // Conveyor Belt
    float texWidth = 1.f / 2.f;
    float texHeight = 1.f / 3.f;

    meshList[GEO_CONVEYORBELT_HORIZONTAL] = MeshBuilder::GenerateQuad("conveyor belt", Color(0.3f, 0.5f, 0.5f));
    meshList[GEO_CONVEYORBELT_HORIZONTAL]->textureID = LoadTGA("Image/Conveyor Belt.tga");
    AdjustTexCoord(GEO_CONVEYORBELT_HORIZONTAL, 0, texWidth, texHeight * 2, texHeight * 3);

    meshList[GEO_CONVEYORBELT_VERTICAL] = MeshBuilder::GenerateQuad("conveyor belt", Color(0.3f, 0.5f, 0.5f));
    meshList[GEO_CONVEYORBELT_VERTICAL]->textureID = LoadTGA("Image/Conveyor Belt.tga");
    AdjustTexCoord(GEO_CONVEYORBELT_VERTICAL, texWidth, texWidth * 2, texHeight, texHeight * 2);

    meshList[GEO_CONVEYORBELT_TOPRIGHT] = MeshBuilder::GenerateQuad("conveyor belt", Color(0.3f, 0.5f, 0.5f));
    meshList[GEO_CONVEYORBELT_TOPRIGHT]->textureID = LoadTGA("Image/Conveyor Belt.tga");
    AdjustTexCoord(GEO_CONVEYORBELT_TOPRIGHT, texWidth, texWidth * 2, texHeight * 2, texHeight * 3);

    meshList[GEO_CONVEYORBELT_BOTRIGHT] = MeshBuilder::GenerateQuad("conveyor belt", Color(0.3f, 0.5f, 0.5f));
    meshList[GEO_CONVEYORBELT_BOTRIGHT]->textureID = LoadTGA("Image/Conveyor Belt.tga");
    AdjustTexCoord(GEO_CONVEYORBELT_BOTRIGHT, texWidth, texWidth * 2, 0, texHeight);

    meshList[GEO_CONVEYORBELT_TOPLEFT] = MeshBuilder::GenerateQuad("conveyor belt", Color(0.3f, 0.5f, 0.5f));
    meshList[GEO_CONVEYORBELT_TOPLEFT]->textureID = LoadTGA("Image/Conveyor Belt.tga");
    AdjustTexCoord(GEO_CONVEYORBELT_TOPLEFT, 0, texWidth, texHeight, texHeight * 2);

    meshList[GEO_CONVEYORBELT_VERTICALEND] = MeshBuilder::GenerateQuad("conveyor belt", Color(0.3f, 0.5f, 0.5f));
    meshList[GEO_CONVEYORBELT_VERTICALEND]->textureID = LoadTGA("Image/Conveyor Belt.tga");
    AdjustTexCoord(GEO_CONVEYORBELT_VERTICALEND, 0, texWidth, 0, texHeight);

    //float texCountWidth = m_dir;
    //float texCountHeight = GetMaxStates() - 1 - GetStateInt();
    //float texWidth = 1.f / 4;
    //float texHeight = 1.f / GetMaxStates();


    meshList[GEO_FLOOR] = MeshBuilder::GenerateQuad("floor", Color(1, 1, 1), 1.f, 4.f);
    meshList[GEO_FLOOR]->textureID = LoadTGA("Image//floor.tga");

    meshList[GEO_TEXT] = MeshBuilder::GenerateText("text", 16, 16);
    meshList[GEO_TEXT]->textureID = LoadTGA("Image//Fonts/Font_FFF Forward.tga");
    meshList[GEO_TEXT]->LoadFontData("Image//Fonts/FontData_FFF Forward.csv");
    meshList[GEO_TEXT]->material.kAmbient.Set(1, 0, 0);

    meshList[GEO_PATHFINDING_NODE] = MeshBuilder::GenerateQuad("pathfinding node", Color(1, 1, 1), 0.25f);

    meshList[GEO_HIGHLIGHTBOX] = MeshBuilder::GenerateQuad("highlight box", Color(1, 1, 1));
    meshList[GEO_HIGHLIGHTBOX]->textureID = LoadTGA("Image//highlight_box.tga");
}

void MeshList::Exit()
{
    for (int i = 0; i < NUM_GEOMETRY; ++i)
    {
        if (meshList[i])
        {
            delete meshList[i];
            meshList[i] = NULL;
        }
    }
}

Mesh* MeshList::GetMesh(GEOMETRY_TYPE geo_type)
{
    return meshList[geo_type];
}

void MeshList::AdjustTexCoord(GEOMETRY_TYPE geo_type, float u1, float u2, float v1, float v2)
{
    if (meshList[geo_type]->textureID > 0)
    {
        TexCoord texCoords[4] = {
            TexCoord(u1, v1),
            TexCoord(u2, v1),
            TexCoord(u2, v2),
            TexCoord(u1, v2)
        };

        glBindBuffer(GL_ARRAY_BUFFER, meshList[geo_type]->vertexBuffer);
        for (unsigned i = 0; i < 4; ++i) {
            glBufferSubData(GL_ARRAY_BUFFER, (sizeof(Vertex)-sizeof(TexCoord)) + (i * sizeof(Vertex)), sizeof(TexCoord), &texCoords[i]);
        }
    }
}