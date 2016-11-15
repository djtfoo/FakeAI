#include "Robot.h"

Robot::Robot() : Entity("Robot")
{
}

Robot::~Robot()
{
}

void Robot::Init()
{
	m_currWaypoint = 0;
}

void Robot::Update(double dt)
{
	// Temporary, just to make robot move away
	//m_pos.x += dt;

	Vector3 dir = (m_beltToFollow->GetNextCheckpoint(m_currWaypoint) - m_pos).Normalized();
	m_pos += dir * dt;

	if ((m_beltToFollow->GetNextCheckpoint(m_currWaypoint) - m_pos).Length() < 1.005)
		++m_currWaypoint;

}

void Robot::Sense(double dt)
{
    m_lifetime += dt;
}

int Robot::Think()
{
    if (m_lifetime >= 120.0)
        return 1;
    else if (1)
        return 2;

    return 0;
}

void Robot::Act(int value)
{
    switch (value)
    {
    case 0:
        //ActShutDown();
        break;

    case 1:
        //ActCheer();
        break;
    }
}

void Robot::SetRobotState(ROBOT_STATE state)
{
    this->m_state = state;
}

Robot::ROBOT_STATE Robot::GetRobotState()
{
    return m_state;
}

void Robot::SetBelt(ConveyorBelt* belt)
{
	m_beltToFollow = belt;
}

void Robot::SetWaypoint(int idx)
{
	m_currWaypoint = idx;
}