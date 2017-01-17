#include "OrnamentBuildingSystem.h"
#include "../SharedData.h"
#include "../MessageBoard/MessageBoard.h"

OrnamentBuildingSystem::OrnamentBuildingSystem()
: x_laneToBlocks(0), x_laneToOrnament(0)
, m_buildingBlocksIndex(1), m_ornamentsIndex(2)
, d_buildingBlocksTimer(0.0), m_completedOrnamentIndex(-1)
, b_justCompleted(false)
{
    for (int i = 0; i < 3; ++i)
    {
        m_buildingBlocks[i] = 0;
        m_ornaments[i] = 0;
    }

    // randomise starting index
    m_buildingBlocksIndex = Math::RandIntMinMax(0, 2);
    m_ornamentsIndex = Math::RandIntMinMax(0, 2);
}

OrnamentBuildingSystem::~OrnamentBuildingSystem()
{
}

void OrnamentBuildingSystem::Init()
{
}

void OrnamentBuildingSystem::Update(double dt)
{
    if (b_justCompleted)
        b_justCompleted = false;

    d_buildingBlocksTimer += dt;

    // IF STACK HAS BEEN EXHAUSTED
    if (d_buildingBlocksTimer >= 20.0)  // robots go to that stack for 20 seconds; think of it as exhausting that stack of blocks
    {
        int newIndex = m_buildingBlocksIndex;
        while (newIndex == m_buildingBlocksIndex)
            newIndex = Math::RandIntMinMax(0, 2);
        
        m_buildingBlocksIndex = newIndex;
        d_buildingBlocksTimer = 0.0;    // reset the timer
    }
    
    // IF ORNAMENT COMPLETED BUILDING
    if (m_ornaments[m_ornamentsIndex]->isComplete())
    {
        // set bool for robots to cheer
        b_justCompleted = true;

        // set index for deliveryman to come
        m_completedOrnamentIndex = m_ornamentsIndex;
    
        // send message to delivery man
        //SharedData::GetInstance()->m_messageBoard->AddMessage(new Message(Message::ORNAMENT_COMPLETE, "Delivery Man", "Ornament", SharedData::GetInstance()->m_clock->GetCurrTimeObject()));
        SharedData::GetInstance()->m_messageBoard->AddMessage(new Message(Message::ORNAMENT_COMPLETE, "Delivery Man", m_ornaments[m_ornamentsIndex], SharedData::GetInstance()->m_clock->GetCurrTimeObject()));

        // change index for robots to build on
        int newIndex = m_ornamentsIndex;
        while (newIndex == m_ornamentsIndex)
            newIndex = Math::RandIntMinMax(0, 2);
    
        m_ornamentsIndex = newIndex;
    }
}

void OrnamentBuildingSystem::Exit()
{
}

void OrnamentBuildingSystem::SetBuildingBlockStack(BuildingBlockStack* buildingBlock)
{
    for (int i = 0; i < 3; ++i)
    {
        if (m_buildingBlocks[i] == 0)
        {
            m_buildingBlocks[i] = buildingBlock;
            break;
        }
    }
}

void OrnamentBuildingSystem::SetOrnament(Ornament* ornament)
{
    for (int i = 0; i < 3; ++i)
    {
        if (m_ornaments[i] == 0)
        {
            m_ornaments[i] = ornament;
            break;
        }
    }
}

void OrnamentBuildingSystem::SetLaneToBlock(int x)
{
    this->x_laneToBlocks = x;
}

void OrnamentBuildingSystem::SetLaneToOrnament(int x)
{
    this->x_laneToOrnament = x;
}

Vector3 OrnamentBuildingSystem::GetBuildingBlockCoord()
{
    return m_buildingBlocks[m_buildingBlocksIndex]->GetPos();
}

Vector3 OrnamentBuildingSystem::GetOrnamentCoord()
{
    return m_ornaments[m_ornamentsIndex]->GetPos();
}

Ornament* OrnamentBuildingSystem::GetCompletedOrnament()
{
    if (m_completedOrnamentIndex == -1)
        return NULL;

    return m_ornaments[m_completedOrnamentIndex];
}

bool OrnamentBuildingSystem::IsJustCompleted()
{
    return b_justCompleted;
}

int OrnamentBuildingSystem::GetXLaneToBlocks()
{
    return x_laneToBlocks;
}

int OrnamentBuildingSystem::GetXLaneToOrnament()
{
    return x_laneToOrnament;
}

void OrnamentBuildingSystem::AddPart()
{
    m_ornaments[m_ornamentsIndex]->AddPart();
}

double OrnamentBuildingSystem::GetBuildingBlocksTimer()
{
    return d_buildingBlocksTimer;
}