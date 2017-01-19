#include "Supervisor.h"

#include "Robot.h"

Supervisor::Supervisor() : Entity("Supervisor")
{
}

Supervisor::~Supervisor()
{
    if (m_pathfinder)
        delete m_pathfinder;
}

void Supervisor::Init()
{
    m_pathfinder = new Pathfinder();
    b_reachedDestination = false;

    m_state = IDLE;
    m_timer = 0;
    m_breakCharge = Math::RandFloatMinMax(-1000, 1000);
    m_atWorkstation = true;
    m_breakDone = false;
    m_inToilet = false;
    m_doOnce = false;
    m_shouldMoveForward = true;
    m_toiletIdx = 0;

    m_workstation = NULL;
    m_toilet = NULL;

}

void Supervisor::SetPos(Vector3 pos)
{
    m_pos = pos;
    m_origSpawn = m_pos;
}

void Supervisor::Update(double dt)
{
    // Update message notification
    if (b_newMsgNotif)
    {
        UpdateMessageNotif(dt);
    }
    if (b_renderAcknowledgeMsg)
    {
        UpdateMessageAcknowledged(dt);
    }

    if (m_state != OFFWORK)
    {
        if (m_state == BREAK)
        {
            if ((m_pos - m_toilet->GetPos()).Length() < 4)  // within toilet range
            {
                if (!m_doOnce)
                {
                    m_toiletIdx = m_toilet->AddToQueue(this);
                    //std::cout << "ADDED" << std::endl;
                    m_doOnce = true;
                }

                Vector3 dir = (m_toilet->GetQueuePosition(m_toiletIdx) - m_pos);
                if (!dir.IsZero())
                    dir.Normalize();

                m_pos += dir * dt;
            }
            else
            {
                //pathfind to toilet

                m_pos += m_vel * dt;
                if (m_pathfinder->hasReachedNode(this->m_pos))
                {
                    // reached destination; can get a part and move on.
                    if (m_pathfinder->hasReachedDestination(this->m_pos))
                    {
                        WhenReachedDestination();
                    }
                    else
                    {
                        WhenReachedPathNode();
                    }
                }

                //Vector3 dir = (m_toilet->GetPos() - m_pos).Normalized();
                //m_pos += dir * dt;
            }

            if ((m_pos - m_toilet->GetPos()).Length() < 0.1)
            {
                m_inToilet = true;
                m_toilet->SetOccupied(true);
            }
            else
                m_inToilet = false;
        }

        if (m_state == IDLE && !m_atWorkstation)
        {
            if ((m_origSpawn - m_pos).Length() > 0.1)
            {
                // pathfind to workstation
                m_pos += m_vel * dt;
                if (m_pathfinder->hasReachedNode(this->m_pos))
                {
                    // reached destination; can get a part and move on.
                    if (m_pathfinder->hasReachedDestination(this->m_pos))
                    {
                        WhenReachedDestination();
                    }
                    else
                    {
                        WhenReachedPathNode();
                    }
                }

                //Vector3 dir = (m_origSpawn - m_pos);
                //
                //if (!dir.IsZero())
                //    dir.Normalize();
                //
                //m_pos += dir * dt;


                if ((m_pos - m_origSpawn).Length() < 0.1)
                {
                    m_atWorkstation = true;
                    m_doOnce = false;
                }
            }
        }
    }
    else if (m_state == OFFWORK)
    {
        if (!m_pathfinder->IsPathEmpty())
        {
            // Walk to door
            m_pos += m_vel * dt;
            if (m_pathfinder->hasReachedNode(this->m_pos))
            {
                // check if reached destination; 
                if (m_pathfinder->hasReachedDestination(this->m_pos))
                {
                    // reached
                    WhenReachedDestination();
                }
                else
                {
                    WhenReachedPathNode();
                }
            }
        }
    }

    switch (m_state)
    {
    case IDLE:
        DoIdle();
        break;

    case PATROL:
        DoPatrol();
        break;

    case BREAK:
        DoBreak();
        break;

    case OFFWORK:
        DoOffWork();
        break;
    }
}

void Supervisor::Sense(double dt)
{
    if (m_state == PATROL)
        m_timer += dt;
    else if (m_state == BREAK && m_inToilet)
        m_timer += dt;
    else if (m_state == IDLE)
    {
        if (m_atWorkstation)
        {
            m_timer += dt;
            if (m_timer > 1)
            {
                m_timer = 0;
                m_breakCharge += Math::RandFloatMinMax(0, 200);
            }
        }
    }
}

int Supervisor::Think()
{
    switch (m_state)
    {

    case IDLE:

        if (!SharedData::GetInstance()->m_clock->GetIsWorkDay() && !SharedData::GetInstance()->m_clock->GetIsWorkStarted())
            return OFFWORK;

        if (m_breakCharge >= 2000)
        {
            int randNum = Math::RandIntMinMax(0, 100);
            if (randNum < 50)
            {
                m_breakCharge = 0;
            }
            else
            {
                m_doOnce = false;
                return BREAK;
            }
        }

        break;

    case PATROL:

        break;

    case BREAK:
        if (m_breakDone)
        {
            m_breakDone = false;
            m_breakCharge = 0;
            return IDLE;
        }
        break;

    case OFFWORK:
        if (SharedData::GetInstance()->m_clock->GetIsWorkDay())
        {
            float randNum = Math::RandFloatMinMax(0, 100);
            if (randNum < 0.1 || SharedData::GetInstance()->m_clock->GetIsWorkStarted())
            {
                m_breakCharge = 0;
                return IDLE;
            }
        }

    }

    return -1;
}

void Supervisor::Act(int value)
{
    switch (value)
    {
    case IDLE:
        SetState(IDLE);
        b_reachedDestination = false;
        //DoIdle();

        // pathfind to workstation
        m_pathfinder->EmptyPath();
        m_pathfinder->ReceiveCurrentPos(Vector3(RoundOff(m_pos.x), RoundOff(m_pos.y), m_pos.z));
        m_pathfinder->ReceiveDestination(m_origSpawn);
        m_pathfinder->FindPathGreedyBestFirst();

        SetVelocity(CheckVelocity(m_pos, m_pathfinder->foundPath.back().GetPosition()));
        SetDirection(CheckDirection(m_vel));
        m_pathfinder->ReceiveDirection(m_dir);
        break;

    case PATROL:
        SetState(PATROL);
        //DoWork();
        SetDirection(CheckDirection(this->m_pos, m_workstation->GetPos()));
        break;

    case BREAK:
        SetState(BREAK);
        b_reachedDestination = false;
        //DoBreak();

        // pathfind to toilet
        m_pathfinder->EmptyPath();
        m_pathfinder->ReceiveCurrentPos(Vector3(RoundOff(m_pos.x), RoundOff(m_pos.y), m_pos.z));
        m_pathfinder->ReceiveDestination(m_toilet->GetPos());
        m_pathfinder->FindPathGreedyBestFirst();

        SetVelocity(CheckVelocity(m_pos, m_pathfinder->foundPath.back().GetPosition()));
        SetDirection(CheckDirection(m_vel));
        m_pathfinder->ReceiveDirection(m_dir);
        break;

    case OFFWORK:
    {
                    SetState(OFFWORK);
                    // pathfind to door
                    m_pathfinder->EmptyPath();
                    m_pathfinder->ReceiveCurrentPos(Vector3(RoundOff(m_pos.x), RoundOff(m_pos.y), m_pos.z));

                    GameObject* door;
                    for (auto i : SharedData::GetInstance()->m_goList)
                    {
                        if (i->GetName() == "Door")
                            door = i;
                    }

                    m_pathfinder->ReceiveDestination(door->GetPos());
                    m_pathfinder->FindPathGreedyBestFirst();

                    SetVelocity(CheckVelocity(m_pos, m_pathfinder->foundPath.back().GetPosition()));
                    SetDirection(CheckDirection(m_vel));
                    m_pathfinder->ReceiveDirection(m_dir);
                    break;
    }
    }
}

void Supervisor::DoIdle()
{
    if ((m_pos - m_origSpawn).Length() < 0.1)
    {
        // Set bool to move back to workplace
        m_atWorkstation = true;
    }
    else
    {
        m_atWorkstation = false;
    }

    if (m_atWorkstation)
        SetDirection(DIR_DOWN);
}

void Supervisor::DoPatrol()
{

}

void Supervisor::DoBreak()
{
    if (m_toilet->CheckIfChange())
    {
        if (m_shouldMoveForward)
        {
            m_toiletIdx = Math::Max(--m_toiletIdx, 0);
            m_shouldMoveForward = false;
        }
    }
    else
    {
        m_shouldMoveForward = true;
    }

    if (m_timer > 4)
    {
        m_breakCharge = 0;
        m_breakDone = true;
        m_timer = 0;
        m_toilet->RemoveFromQueue();
        //std::cout << "POPPED" << std::endl;
        m_shouldMoveForward = true;

        m_toilet->SetOccupied(false);
    }
}

void Supervisor::DoOffWork()
{
}

bool Supervisor::IsOnBreak()
{
    if (m_state == BREAK)
        return true;

    return false;
}


void Supervisor::SetWorkstation(Workstation* station)
{
    m_workstation = station;
}

Supervisor::SUPERVISOR_STATE Supervisor::GetState()
{
    return m_state;
}

void Supervisor::SetState(SUPERVISOR_STATE state)
{
    this->m_state = state;
    this->SetSprite();
}

int Supervisor::GetStateInt()
{
    return m_state;
}

int Supervisor::GetMaxStates()
{
    return SUPERVISOR_STATES_TOTAL;
}

void Supervisor::SetToilet(Toilet* toilet)
{
    m_toilet = toilet;
}

Toilet* Supervisor::GetToilet()
{
    return m_toilet;
}

double Supervisor::GetBreakCharge()
{
    return m_breakCharge;
}

Pathfinder* Supervisor::GetPathfinder()
{
    return m_pathfinder;
}