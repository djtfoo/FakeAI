#include "Robot.h"

Robot::Robot() : Entity("Robot")
{
}

Robot::~Robot()
{
}

void Robot::Init()
{
    m_lifetime = 0.0;
    d_cheerCounter = 0.0;

	m_currWaypoint = 0;
    b_workedOn = false;
    b_active = true;

    m_state = INCOMPLETE_1;
    m_vel.Set(0, 0, 0);

    m_stateBeforeCheer = CHEER;
    b_ornamentCompleted = false;
}

void Robot::Update(double dt)
{
    switch (m_state)
    {
    case INCOMPLETE_1:
    case INCOMPLETE_2:
    case INCOMPLETE_3:
        if (!b_workedOn)
        {
            if (m_vel.IsZero())
            {
                m_vel = CheckVelocity(m_pos, m_beltToFollow->GetNextCheckpoint(m_currWaypoint));
                SetDirection(CheckDirection(m_vel));
            }
            //Vector3 dir = (m_beltToFollow->GetNextCheckpoint(m_currWaypoint) - m_pos).Normalized();
            m_pos += m_vel * dt;

            if ((m_beltToFollow->GetNextCheckpoint(m_currWaypoint) - m_pos).Length() < 0.1)
            {
                ++m_currWaypoint;
                //m_vel = (m_beltToFollow->GetNextCheckpoint(m_currWaypoint) - m_pos).Normalized();
                m_vel = CheckVelocity(m_pos, m_beltToFollow->GetNextCheckpoint(m_currWaypoint));
                SetDirection(CheckDirection(m_vel));
            }
                
        }
        break;

    case STARTUP:
        if (!m_vel.IsZero())
            m_vel.SetZero();
        break;
        
    case WORK_WITHOUTPART:
        m_pos += m_vel * dt;
        break;

    case WORK_WITHPART:
        m_pos += m_vel * dt;
        break;

    case CHEER:
        break;

    case SHUTDOWN:
        break;
    }


}

void Robot::Sense(double dt)
{
    if (m_state > INCOMPLETE_3)
        m_lifetime += dt;

    if (m_state == CHEER)
        d_cheerCounter += dt;

    //if (ornament completed)
    //{
    //    b_ornamentCompleted = true;
    //}
}

int Robot::Think()
{
    switch (m_state)
    {
    case STARTUP:
        if (m_lifetime >= 3.0)
            return WORK_WITHOUTPART;
        break;

    case WORK_WITHOUTPART:
        if (m_lifetime >= 120.0)    // change to Markov
            return SHUTDOWN;

        if (b_ornamentCompleted)
            return CHEER;
        break;

    case WORK_WITHPART:
        if (m_lifetime >= 120.0)    // change to Markov
            return SHUTDOWN;

        if (b_ornamentCompleted)
            return CHEER;
        break;

    case CHEER:
        if (d_cheerCounter >= 5.0)
            return WORK_WITHPART;    // temp default
        break;

    case SHUTDOWN:
        break;
    }

    return -1;
}

void Robot::Act(int value)
{
    switch (value)
    {
    case WORK_WITHOUTPART:
        SetState(WORK_WITHOUTPART);
        // Pathfind to the building block stack
        break;

    case WORK_WITHPART: // when robot finished cheering and is to continue work
        d_cheerCounter = 0.0;
        SetState(m_stateBeforeCheer);
        // set velocity
        break;

    case CHEER: // going into cheer state
        SetState(CHEER);
        m_stateBeforeCheer = m_state;
        b_ornamentCompleted = false;
        m_vel.SetZero();
        break;

    case SHUTDOWN:
        SetState(SHUTDOWN);
        m_vel.SetZero();
        break;
    }
}

void Robot::SetBelt(ConveyorBelt* belt)
{
	m_beltToFollow = belt;
}

void Robot::SetWaypoint(int idx)
{
	m_currWaypoint = idx;
}

void Robot::SetWorkedOn(bool status)
{
    b_workedOn = status;
}

Robot::ROBOT_STATE Robot::GetState()
{
    return m_state;
}

void Robot::SetState(ROBOT_STATE state)
{
    this->m_state = state;
    this->SetSprite();
}

int Robot::GetStateInt()
{
    return m_state;
}

int Robot::GetMaxStates()
{
    return ROBOT_STATES_TOTAL;
}