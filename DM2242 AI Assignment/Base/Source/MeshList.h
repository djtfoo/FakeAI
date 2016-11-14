#ifndef MESHLIST_H
#define MESHLIST_H

#include "Mesh.h"

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