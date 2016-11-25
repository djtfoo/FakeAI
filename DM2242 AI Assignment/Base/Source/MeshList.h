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

    // Game Objects
    GEO_DELIVERYTRUCK,
    GEO_CONVEYORBELT,
    GEO_TOILET,
    GEO_WORKSTATION,

    //GEO_SCRAP_PIECE,
    GEO_SCRAP_STACK,

    GEO_ROBOT_PART_PACKAGE,
    
    // TO REMOVE
    //GEO_ROBOT_STAGE1,
    //GEO_ROBOT_STAGE2,
    //GEO_ROBOT_STAGE3,

    GEO_BUILDINGBLOCK_STACK,
    GEO_ORNAMENT,

    // Generic/Debug
    GEO_FLOOR,
    GEO_TEXT,
    GEO_PATHFINDING_NODE,
    
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
};

#endif