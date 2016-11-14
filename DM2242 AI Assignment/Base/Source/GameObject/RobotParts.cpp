#include "RobotParts.h"

RobotPart::RobotPart() : GameObject("RobotPart")
{
}

RobotPart::~RobotPart()
{
}

void RobotPart::Init()
{}

void RobotPart::Update(double dt)
{
    // Temporary
    m_pos.x += dt;
}

int RobotPart::GetCost()
{
    return m_cost;
}

void RobotPart::SetType(ROBOT_PART type)
{
    m_type = type;

    switch (type)
    {
    case HEAD:
        m_cost = 2;
        break;

    case BODY:
        m_cost = 4;
        break;

    case LIMB:
        m_cost = 3;
        break;

    case MICROCHIP:
        m_cost = 1;
        break;
    }
}

RobotPart::ROBOT_PART RobotPart::GetType()
{
    return m_type;
}