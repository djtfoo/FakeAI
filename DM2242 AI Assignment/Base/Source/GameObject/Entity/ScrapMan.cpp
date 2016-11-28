#include "ScrapMan.h"
#include "../../SharedData.h"

ScrapMan::ScrapMan() : Entity("Scrap Man")
{
}

ScrapMan::~ScrapMan()
{
    if (m_pathfinder)
        delete m_pathfinder;
}

void ScrapMan::Init()
{
    d_breakCharge = Math::RandFloatMinMax(-1000, 1000);
    d_timerCounter = 0.0;

    m_pathfinder = new Pathfinder();

    b_reachedDestination = false;

    m_dir = DIR_DOWN;
    m_state = IDLE;

    m_robotToPickUp = NULL;

    m_pile = NULL;

    m_breakDone = false;
    m_inToilet = false;
    m_doOnce = false;
    m_toiletIdx = -1;
    m_origSpawn.SetZero();
    m_toilet = NULL; 
    m_atWorkstation = true;
}

void ScrapMan::SetPos(Vector3 pos)
{
    m_pos = pos;
    m_origSpawn = m_pos;
}

void ScrapMan::Update(double dt)
{
    switch (m_state)
    {
    case IDLE:
        // Move back to orig pos
        if ((m_origSpawn - m_pos).Length() > 0.1)
        {
            m_atWorkstation = false;
            Vector3 dir = (m_origSpawn - m_pos);

            if (!dir.IsZero())
                dir.Normalize();

            m_pos += dir * dt;

            if ((m_pos - m_origSpawn).Length() < 0.1)
            {
                m_atWorkstation = true;
            }
        }
        break;

    case COLLECT_ROBOT:
        m_pos += m_vel * dt;
        if (m_pathfinder->hasReachedNode(this->m_pos))
        {
            // reached destination; can get a part and move on.
            if (m_pathfinder->hasReachedDestination(this->m_pos))
            {
                m_pathfinder->foundPath.pop_back();

                m_vel.SetZero();

                b_reachedDestination = true;
            }
            else
            {
                m_pathfinder->foundPath.pop_back();

                SetVelocity(CheckVelocity(m_pos, m_pathfinder->foundPath.back().GetPosition()));
                SetDirection(CheckDirection(m_vel));
            }
        }
        break;

    case BREAKDOWN_ROBOT:
        if (!b_reachedDestination)
        {
            // move back to workstation
            m_pos += m_vel * dt;
            if (m_pathfinder->hasReachedNode(this->m_pos))
            {
                // reached destination; can get a part and move on.
                if (m_pathfinder->hasReachedDestination(this->m_pos))
                {
                    m_pathfinder->foundPath.pop_back();

                    m_vel.SetZero();
                    SetDirection(DIR_RIGHT);

                    b_reachedDestination = true;
                }
                else
                {
                    m_pathfinder->foundPath.pop_back();

                    SetVelocity(CheckVelocity(m_pos, m_pathfinder->foundPath.back().GetPosition()));
                    SetDirection(CheckDirection(m_vel));
                }
            }

            m_robotToPickUp->SetPos(this->m_pos);
        }

        break;

    case BREAK:
        // Check if toilet is close, if so add to queue and walk to it
        if ((m_pos - m_toilet->GetPos()).Length() < 3)
        {
            if (!m_doOnce)
            {
                m_toiletIdx = m_toilet->AddToQueue(this);
                std::cout << "ADDED" << std::endl;
                m_doOnce = true;
            }

            Vector3 dir = (m_toilet->GetQueuePosition(m_toiletIdx) - m_pos);
            if (!dir.IsZero())
                dir.Normalize();

            m_pos += dir * dt;
        }
        else
        {
            // Move to general area of toilet
            Vector3 dir = (m_toilet->GetPos() - m_pos).Normalized();
            m_pos += dir * dt;
        }

        // If close to toilet set inToilet to true
        if ((m_pos - m_toilet->GetPos()).Length() < 0.1)
        {
            m_inToilet = true;
            m_toilet->SetOccupied(true);
        }
        else
            m_inToilet = false;

        break;
    }
}

void ScrapMan::Sense(double dt)
{
    if (m_state == IDLE && m_robotToPickUp == NULL)
    {
        // Update breakcharge
        if (m_atWorkstation)
        {
            d_timerCounter += dt;
            if (d_timerCounter > 1)
            {
                d_timerCounter = 0;
                d_breakCharge += Math::RandFloatMinMax(0, 200);
                std::cout << d_breakCharge << std::endl;
            }
        }
        // check whether there's any broken down Robots
        for (std::vector<GameObject*>::iterator it = SharedData::GetInstance()->m_goList.begin(); it != SharedData::GetInstance()->m_goList.end(); ++it)
        {
            GameObject* go = (GameObject*)(*it);
            if (go->IsActive() && go->GetName() == "Robot")
            {
                Robot* robot = dynamic_cast<Robot*>(go);
                if (robot->GetState() == Robot::SHUTDOWN)
                    m_robotToPickUp = robot;
            }

            if (m_robotToPickUp != NULL)
                break;
        }
    }

    if (m_state == BREAKDOWN_ROBOT && b_reachedDestination)
    {
        d_timerCounter += dt;
    }

    if (m_state == BREAK && m_inToilet)
        d_timerCounter += dt;
}

int ScrapMan::Think()
{
    if (m_robotToPickUp != NULL && m_state == IDLE)
    {
        return COLLECT_ROBOT;
    }

    if (b_reachedDestination && m_state == COLLECT_ROBOT)
        return BREAKDOWN_ROBOT;

    if (d_timerCounter > 5.0 && m_state == BREAKDOWN_ROBOT)
    {
        // finish breaking down Robot
        m_robotToPickUp->SetInactive();
        m_robotToPickUp = NULL;
        m_pile->SetScrapQuantity(m_pile->GetScrapQuantity() + 1);
        return IDLE;
    }

    if (m_state == IDLE && d_breakCharge > 2000)
    {
        m_doOnce = false;
        return BREAK;
    }

    if (m_state == BREAK)
    {
        if (m_breakDone)
        {
            m_breakDone = false;
            d_breakCharge = 0;
            return IDLE;
        }
        else
        {
            return BREAK;
        }
    }

    return -1;
}

void ScrapMan::Act(int value)
{
    switch (value)
    {
    case IDLE:
        SetState(IDLE);
        b_reachedDestination = false;
        d_timerCounter = 0.0;

        break;

    case COLLECT_ROBOT:
    {
                          SetState(COLLECT_ROBOT);

                          // Pathfind to the shutdown robot
                          m_pathfinder->ReceiveCurrentPos(this->m_pos);
                          Vector3 pos = m_robotToPickUp->GetPos();
                          m_pathfinder->ReceiveDestination(Vector3(RoundOff(pos.x), RoundOff(pos.y), pos.z));
                          m_pathfinder->FindPathGreedyBestFirst();

                          SetVelocity(CheckVelocity(m_pos, m_pathfinder->foundPath.back().GetPosition()));
                          SetDirection(CheckDirection(m_vel));
                          break;
    }

    case BREAKDOWN_ROBOT:
    {
                            SetState(BREAKDOWN_ROBOT);
                            b_reachedDestination = false;

                            // Pathfind to the ScrapMan's workstation
                            m_pathfinder->ReceiveCurrentPos(this->m_pos);
                            m_pathfinder->ReceiveDestination(m_pile->GetPos() + Vector3(-1, 0, 0));
                            m_pathfinder->FindPathGreedyBestFirst();

                            SetVelocity(CheckVelocity(m_pos, m_pathfinder->foundPath.back().GetPosition()));
                            SetDirection(CheckDirection(m_vel));

                            break;
    }

    case BREAK:
        SetState(BREAK);
        if (m_toilet->CheckIfChange())
            m_toiletIdx = Math::Max(--m_toiletIdx, 0);

        if (d_timerCounter > 4)
        {
            d_breakCharge = 0;
            m_breakDone = true;
            d_timerCounter = 0;
            m_toilet->RemoveFromQueue();
            std::cout << "POPPED" << std::endl;

            m_toilet->SetOccupied(false);
        }
        break;
    }
}

ScrapMan::SCRAPMAN_STATE ScrapMan::GetState()
{
    return m_state;
}

void ScrapMan::SetState(SCRAPMAN_STATE state)
{
    this->m_state = state;
    this->SetSprite();
}

int ScrapMan::GetStateInt()
{
    return m_state;
}

int ScrapMan::GetMaxStates()
{
    return SCRAPMAN_STATES_TOTAL;
}

void ScrapMan::AssignScrapPile(ScrapPile* pile)
{
    m_pile = pile;
}

void ScrapMan::SetToilet(Toilet* toilet)
{
    m_toilet = toilet;
}

Toilet* ScrapMan::GetToilet()
{
    return m_toilet;
}