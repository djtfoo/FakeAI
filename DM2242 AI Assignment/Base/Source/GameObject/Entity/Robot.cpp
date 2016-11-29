#include "Robot.h"
#include "../../SharedData.h"

Robot::Robot() : Entity("Robot")
{
}

Robot::~Robot()
{
    if (m_pathfinder)
        delete m_pathfinder;
}

void Robot::Init()
{
    m_pathfinder = new Pathfinder();

    m_lifetime = 0.0;
    d_timerCounter = 0.0;

    d_timerForShutDown = 0.0;
    b_toShutDown = false;

    b_reachedDestination = false;

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
        if (!b_reachedDestination)
        {
            m_pos += m_vel * dt;
            if (m_pathfinder->hasReachedNode(this->m_pos))
            {
                //m_pos = m_pathfinder->foundPath.back().GetPosition();   // set to the grid so it's less marginal error

                // reached destination; can get a part and move on.
                if (m_pathfinder->hasReachedDestination(this->m_pos))
                {
                    m_pos = m_pathfinder->foundPath.back().GetPosition();

                    m_pathfinder->foundPath.pop_back();

                    m_vel.SetZero();
                    SetDirection(DIR_UP);

                    b_reachedDestination = true;
                }
                else
                {
                    m_pos = m_pathfinder->foundPath.back().GetPosition();

                    m_pathfinder->foundPath.pop_back();

                    SetVelocity(CheckVelocity(m_pos, m_pathfinder->foundPath.back().GetPosition()) );
                    SetDirection(CheckDirection(m_vel));
                }
            }
        }
        break;

    case WORK_WITHPART:
        if (!b_reachedDestination)
        {
            m_pos += m_vel * dt;
            if (m_pathfinder->hasReachedNode(this->m_pos))
            {
                // reached destination; can get a part and move on.
                if (m_pathfinder->hasReachedDestination(this->m_pos))
                {
                    m_pos = m_pathfinder->foundPath.back().GetPosition();

                    m_pathfinder->foundPath.pop_back();

                    m_vel.SetZero();
                    SetDirection(DIR_DOWN);

                    b_reachedDestination = true;
                }
                else
                {
                    m_pos = m_pathfinder->foundPath.back().GetPosition();

                    m_pathfinder->foundPath.pop_back();

                    SetVelocity(CheckVelocity(m_pos, m_pathfinder->foundPath.back().GetPosition()) );
                    SetDirection(CheckDirection(m_vel));
                }
            }
        }
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

    if ((m_state == WORK_WITHOUTPART || m_state == WORK_WITHPART) && b_reachedDestination)
        d_timerCounter += dt;

    if (m_state == CHEER)
        d_timerCounter += dt;

    // for checking if going into cheer state
    if (SharedData::GetInstance()->m_ornamentSystem->IsJustCompleted())
    {
        b_ornamentCompleted = true;
    }

    if (m_lifetime >= 57.0)
    {
        d_timerForShutDown += dt;
        if (d_timerForShutDown >= 3.0)
        {
            d_timerForShutDown = 0.0;

            int randVal = Math::RandIntMinMax(60, 100);
            if (randVal < m_lifetime)
                b_toShutDown = true;
        }
    }
}

int Robot::Think()
{
    switch (m_state)
    {
    case STARTUP:
        if (m_lifetime >= 3.0)
        {
            m_pos = m_beltToFollow->GetNextCheckpoint(m_currWaypoint);
            m_pathfinder->EmptyPath();
            return WORK_WITHOUTPART;
        }
        break;

    case WORK_WITHOUTPART:
        if (b_toShutDown)    // change to Markov
            return SHUTDOWN;

        if (d_timerCounter >= 0.5)
            return WORK_WITHPART;

        if (b_ornamentCompleted && m_lifetime > 10.0)   // so they don't cheer so early
            return CHEER;
        break;

    case WORK_WITHPART:
        if (b_toShutDown)    // change to Markov
            return SHUTDOWN;

        if (d_timerCounter >= 0.5)
        {
            SharedData::GetInstance()->m_ornamentSystem->AddPart();
            return WORK_WITHOUTPART;
        }

        if (b_ornamentCompleted)
            return CHEER;
        break;

    case CHEER:
        if (b_toShutDown)    // change to Markov
            return SHUTDOWN;

        if (d_timerCounter >= 4.0)
            return m_stateBeforeCheer;
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
        d_timerCounter = 0.0;
        b_reachedDestination = false;
        SetState(WORK_WITHOUTPART);
        
        if (m_pathfinder->IsPathEmpty())
        {
            // Pathfind to the building block stack
            m_pathfinder->ReceiveCurrentPos(Vector3(RoundOff(m_pos.x), RoundOff(m_pos.y), m_pos.z));
            m_pathfinder->ReceiveDestination(SharedData::GetInstance()->m_ornamentSystem->GetBuildingBlockCoord() + Vector3(0, -1, 0));
            m_pathfinder->FindPathGreedyBestFirst();
        }

        SetVelocity(CheckVelocity(m_pos, m_pathfinder->foundPath.back().GetPosition()));
        SetDirection(CheckDirection(m_vel));
        break;

    case WORK_WITHPART: // when robot finished cheering and is to continue work
        d_timerCounter = 0.0;
        b_reachedDestination = false;
        SetState(WORK_WITHPART);

        if (m_pathfinder->IsPathEmpty())
        {
            // Pathfind to the ornament block stack
            m_pathfinder->ReceiveCurrentPos(Vector3(RoundOff(m_pos.x), RoundOff(m_pos.y), m_pos.z));
            m_pathfinder->ReceiveDestination(SharedData::GetInstance()->m_ornamentSystem->GetOrnamentCoord() + Vector3(0, 1, 0));
            m_pathfinder->FindPathGreedyBestFirst();
        }

        SetVelocity(CheckVelocity(m_pos, m_pathfinder->foundPath.back().GetPosition()));
        SetDirection(CheckDirection(m_vel));
        break;

    case CHEER: // going into cheer state
        m_stateBeforeCheer = m_state;
        SetState(CHEER);
        b_ornamentCompleted = false;
        m_vel.SetZero();

        m_pathfinder->EmptyPath();
        break;

    case SHUTDOWN:
        SetState(SHUTDOWN);
        m_vel.SetZero();

        b_toShutDown = false;
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

Vector3 Robot::GetWaypoint()
{
    return m_beltToFollow->GetNextCheckpoint(m_currWaypoint);
}

void Robot::SetWorkedOn(bool status)
{
    b_workedOn = status;
}

bool Robot::IsWorkedOn()
{
    return b_workedOn;
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

double Robot::GetLifetime()
{
    return m_lifetime;
}

Pathfinder* Robot::GetPathfinder()
{
    return m_pathfinder;
}