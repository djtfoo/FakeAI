#ifndef MESHLIST_H
#define MESHLIST_H

#include "Mesh.h"

enum GEOMETRY_TYPE
{
    // Entities
    GEO_MACHINE,
    //GEO_MACHINE_PRODUCTION,
    //GEO_MACHINE_BROKEN,
    //GEO_MACHINE_WAITFORREFILL,

    GEO_WORKER,
    //GEO_WORKER_WORKING,

    GEO_MAINTENANCEMAN,
    GEO_ROBOT,
    GEO_SCRAPMAN,
    GEO_DELIVERYMAN,

    // Conveyor Belt
    GEO_CONVEYORBELT_HORIZONTAL,
    GEO_CONVEYORBELT_VERTICAL,
    GEO_CONVEYORBELT_TOPRIGHT,
    GEO_CONVEYORBELT_BOTRIGHT,
    GEO_CONVEYORBELT_TOPLEFT,
    GEO_CONVEYORBELT_VERTICALEND,

    // Game Objects
    GEO_DELIVERYTRUCK,
    GEO_TOILET,
    GEO_WORKSTATION,

    GEO_SCRAP_PILE,

    GEO_ROBOT_PART_PACKAGE,

    GEO_BUILDINGBLOCK_STACK,
    GEO_ORNAMENT,

    // Generic Scene
    GEO_FLOOR,
    GEO_CLOCK,
    GEO_CLOCKBASE,
    GEO_CLOCKMINUTE,
    GEO_CLOCKHOUR,
    GEO_MESSAGEBOARD,
    GEO_MESSAGEBOARD_TEXT,
    GEO_TEXT,

    // Debug
    GEO_PATHFINDING_NODE,
    GEO_HIGHLIGHTBOX,
    
    NUM_GEOMETRY,
};

class MeshList
{
public:
    MeshList();
    ~MeshList();

    void Init();
    void Exit();

    Mesh* meshList[NUM_GEOMETRY];
    Mesh* GetMesh(GEOMETRY_TYPE geo_type);

    void AdjustTexCoord(GEOMETRY_TYPE geo_type, float u1 = 0.f, float u2 = 1.f, float v1 = 0.f, float v2 = 1.f);
};

#endif