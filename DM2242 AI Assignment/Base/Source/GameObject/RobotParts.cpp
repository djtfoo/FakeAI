#include "RobotParts.h"

RobotPart::RobotPart() : GameObject("RobotPart")
{
}

RobotPart::~RobotPart()
{
}

void RobotPart::Init()
{
	m_currWaypoint = 0;
}

void RobotPart::Update(double dt)
{
    // Temporary
    // m_pos.x += dt;

	Vector3 dir = (m_beltToFollow->GetNextCheckpoint(m_currWaypoint) - m_pos).Normalized();
	m_pos += dir * dt;

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

void RobotPart::SetBelt(ConveyorBelt* belt)
{
	m_beltToFollow = belt;
}

void RobotPart::SetWaypoint(int idx)
{
	m_currWaypoint = idx;
}