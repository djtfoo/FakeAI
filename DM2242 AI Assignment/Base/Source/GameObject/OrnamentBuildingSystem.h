#ifndef ORNAMENT_BUILDING_SYSTEM
#define ORNAMENT_BUILDING_SYSTEM

#include "BuildingBlockStack.h"
#include "Ornament.h"

class OrnamentBuildingSystem
{
    BuildingBlockStack* m_buildingBlocks[3];
    Ornament* m_ornaments[3];

    int x_laneToBlocks;
    int x_laneToOrnament;

    int m_buildingBlocksIndex;  // which stack the robots are to set their destination to
    int m_ornamentsIndex;       // which ornament the robots to build

    double d_buildingBlocksTimer;   // how long to take from a stack for, then change
    int m_completedOrnamentIndex;   // which ornament the deliveryman to take from

public:
    OrnamentBuildingSystem();
    ~OrnamentBuildingSystem();

    void Init();
    void Update(double dt);
    void Exit();

    void SetBuildingBlockStack(BuildingBlockStack* buildingBlock);
    void SetOrnament(Ornament* ornament);

    void SetLaneToBlock(int x);
    void SetLaneToOrnament(int x);
    
    // FOR ROBOT USE
    Vector3 GetBuildingBlockCoord();
    Vector3 GetOrnamentCoord();

    Ornament* GetCompletedOrnament();   // return 0 if no completed ornament available

    int GetXLaneToBlocks();
    int GetXLaneToOrnament();

    void AddPart();
};

#endif