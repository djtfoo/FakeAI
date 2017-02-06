#include "Supervisor.h"

#include "Robot.h"

Supervisor::Supervisor() : Entity("Supervisor"), m_MessageToSend(Message::MESSAGE_TYPES_TOTAL), b_decisionMade(false), d_decisionTimer(0.0), b_shouldMakeDecision(true)
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

    case MAKEDECISION:
        DoMakeDecision();
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
        d_decisionTimer += dt;
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

        // If off-work, immediately acknowledge all messages and reset all related variables
        if (!SharedData::GetInstance()->m_clock->GetIsWorkDay() && !SharedData::GetInstance()->m_clock->GetIsWorkStarted())
        {
            for (int i = 0; i < 5; ++i)
            {
                Message* retrievedMsg = this->ReadMessageBoard(SharedData::GetInstance()->m_messageBoard);

                // Check if retrieved message is invalid
                if (retrievedMsg)
                {
                    AcknowledgeMessage();
                }
            }
            return OFFWORK;
        }

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

        // Make Decision
        if (b_shouldMakeDecision && !b_decisionMade && d_decisionTimer > 15)
        {
            d_decisionTimer = 0;
            return MAKEDECISION;
        }

        // Check for urgency complete message
        // Read Messages
        if (true)
        {
            // Msg can only be acknowledged by supervisor
            bool shouldAcknowledged = true;

            for (int i = 0; i < SharedData::GetInstance()->m_goList.size(); ++i)
            {
                if (!SharedData::GetInstance()->m_goList[i]->IsEntity())
                    continue;

                Entity* checkEntity = dynamic_cast<Entity*>(SharedData::GetInstance()->m_goList[i]);

                while (checkEntity->GetTempRole())
                {
                    checkEntity = checkEntity->GetTempRole();
                }

                if (checkEntity->GetName() == "Worker" || checkEntity->GetName() == "Scrap Man" || checkEntity->GetName() == "Maintenance Man")
                {
                    if (checkEntity->GetUrgencyChanged() == true)
                        shouldAcknowledged = false;
                }
            }

            if (shouldAcknowledged)
            {
                Message* retrievedMsg = this->ReadMessageBoard(SharedData::GetInstance()->m_messageBoard);

                // Check if retrieved message is invalid
                if (retrievedMsg)
                {
                    AcknowledgeMessage();
                }

                b_shouldMakeDecision = true;
            }
            else
            {
                b_shouldMakeDecision = false;
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

    case MAKEDECISION:

        // If off-work, immediately acknowledge all messages and reset all related variables
        if (!SharedData::GetInstance()->m_clock->GetIsWorkDay() && !SharedData::GetInstance()->m_clock->GetIsWorkStarted())
        {
            for (int i = 0; i < 5; ++i)
            {
                Message* retrievedMsg = this->ReadMessageBoard(SharedData::GetInstance()->m_messageBoard);

                // Check if retrieved message is invalid
                if (retrievedMsg)
                {
                    AcknowledgeMessage();
                }
            }
            return OFFWORK;
        }

        if (b_decisionMade)
        {
            // Read Messages
            if (b_newMsgNotif && d_msgNotifTimer >= 2.0)
            {
                // Msg can only be acknowledged by supervisor
                bool shouldAcknowledged = true;

                for (int i = 0; i < SharedData::GetInstance()->m_goList.size(); ++i)
                {
                    if (!SharedData::GetInstance()->m_goList[i]->IsEntity())
                        continue;

                    Entity* checkEntity = dynamic_cast<Entity*>(SharedData::GetInstance()->m_goList[i]);

                    while (checkEntity->GetTempRole())
                    {
                        checkEntity = checkEntity->GetTempRole();
                    }

                    if (checkEntity->GetName() == "Worker" || checkEntity->GetName() == "Scrap Man" || checkEntity->GetName() == "Maintenance Man")
                    {
                        if (checkEntity->GetUrgencyChanged() == false)
                            shouldAcknowledged = false;
                    }
                }

                if (shouldAcknowledged)
                {
                    Message* retrievedMsg = this->ReadMessageBoard(SharedData::GetInstance()->m_messageBoard);

                    // Check if retrieved message is invalid
                    if (retrievedMsg)
                    {
                        AcknowledgeMessage();
                    }

                    SharedData::GetInstance()->m_messageBoard->AddMessage(new Message(Message::COMPLETED_URGENCY_CHANGE, "Humans", this, SharedData::GetInstance()->m_clock->GetCurrTimeObject()));

                    d_decisionTimer = 0;
                    b_decisionMade = false;
                    return IDLE;
                }
                else
                {
                    d_decisionTimer = 0;
                }  
            }
        }
        else
        {
            d_decisionTimer = 0;
            b_decisionMade = false;
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

    case MAKEDECISION:
        SetState(MAKEDECISION);
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

                    b_decisionMade = false;
                    b_shouldMakeDecision = true;
                    d_decisionTimer = 0.0;

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
        m_shouldMoveForward = true;

        m_toilet->SetOccupied(false);
    }
}

void Supervisor::DoMakeDecision()
{
    if (!b_decisionMade)
    {
        // Urgency 
        double averageInactiveLevel = 0;
        int num = 0;

        for (int i = 0; i < SharedData::GetInstance()->m_goList.size(); ++i)
        {
            if (!SharedData::GetInstance()->m_goList[i]->IsEntity())
                continue;

            Entity* checkEntity = dynamic_cast<Entity*>(SharedData::GetInstance()->m_goList[i]);

            while (checkEntity->GetTempRole())
            {
                checkEntity = checkEntity->GetTempRole();
            }

            if (checkEntity->GetName() == "Worker" || checkEntity->GetName() == "Scrap Man" || checkEntity->GetName() == "Maintenance Man")
            {
                averageInactiveLevel += checkEntity->GetInactiveLevel();
                num++;
            }
        }

        averageInactiveLevel = averageInactiveLevel / num;

        // Could be fuzzy
        if (averageInactiveLevel > 7.5)
        {
            m_MessageToSend = Message::INCREASE_URGENCY;
            b_decisionMade = true;
        }
        else if (averageInactiveLevel < 2.5)
        {
            m_MessageToSend = Message::DECREASE_URGENCY;
            b_decisionMade = true;
        }
        else
        {
            m_MessageToSend = Message::MESSAGE_TYPES_TOTAL;
            b_decisionMade = false;
        }

        // Send message
        if (m_MessageToSend != Message::MESSAGE_TYPES_TOTAL)
            SharedData::GetInstance()->m_messageBoard->AddMessage(new Message(m_MessageToSend, "Humans", this, SharedData::GetInstance()->m_clock->GetCurrTimeObject()));
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