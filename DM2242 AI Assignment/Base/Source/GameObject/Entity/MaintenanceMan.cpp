#include "MaintenanceMan.h"

#include "../../SharedData.h"

MaintenanceMan::MaintenanceMan() : Entity("Maintenance Man")
{
}

MaintenanceMan::~MaintenanceMan()
{
    if (m_pathfinder)
        delete m_pathfinder;
}

void MaintenanceMan::Init()
{
    m_pathfinder = new Pathfinder();
    b_reachedDestination = false;

    m_breakCharge = Math::RandFloatMinMax(-1000, 1000);
    m_timer = 0;
    m_state = IDLE;
    m_targetMachine = NULL;
    m_doingWork = false;
    m_breakDone = false;
    m_inToilet = false;
    m_doOnce = false;
    m_toiletIdx = -1;
    m_shouldMoveForward = true;
    m_toilet = NULL;

    randNum = 0;
}

void MaintenanceMan::SetPos(Vector3 pos)
{
    m_pos = pos;
    m_origSpawn = m_pos;
}

void MaintenanceMan::Update(double dt)
{
    if (m_state == BREAK)
    {
        // Check if toilet is close, if so add to queue and walk to it
        if ((m_pos - m_toilet->GetPos()).Length() < 4)  // within toilet range
        {
            if (!m_doOnce && m_state == BREAK)
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
                    m_pos = m_pathfinder->foundPath.back().GetPosition();

                    m_pathfinder->foundPath.pop_back();

                    m_vel.SetZero();

                    b_reachedDestination = true;
                }
                else
                {
                    m_pos = m_pathfinder->foundPath.back().GetPosition();

                    m_pathfinder->foundPath.pop_back();

                    SetVelocity(CheckVelocity(m_pos, m_pathfinder->foundPath.back().GetPosition()));
                    SetDirection(CheckDirection(m_vel));
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
    else if (m_targetMachine)   // there is work to do
    {
        if (!m_doingWork)
        {
            // Move Towards TargetMachine, then do work
            //Vector3 dir = (m_targetMachine->GetPos() - m_pos).Normalized();
            //m_pos += dir * dt;

            // follow found path to TargetMachine
            m_pos += m_vel * dt;
            if (m_pathfinder->hasReachedNode(this->m_pos))
            {
                // reached destination; can get a part and move on.
                if (m_pathfinder->hasReachedDestination(this->m_pos))
                {
                    m_pos = m_pathfinder->foundPath.back().GetPosition();

                    m_pathfinder->foundPath.pop_back();

                    m_vel.SetZero();

                    b_reachedDestination = true;
                    m_doingWork = true;
                }
                else
                {
                    m_pos = m_pathfinder->foundPath.back().GetPosition();

                    m_pathfinder->foundPath.pop_back();

                    SetVelocity(CheckVelocity(m_pos, m_pathfinder->foundPath.back().GetPosition()));
                    SetDirection(CheckDirection(m_vel));
                }
            }

            if ((m_pos - m_targetMachine->GetPos()).Length() < 1.001)
            {
                m_doingWork = true;
            }
        }
    }
    else    // idle state
    {
        // Move to workstation if no TargetMachine
        if ((m_origSpawn - m_pos).Length() > 0.1)
        {
            // pathfind to workstation
            m_pos += m_vel * dt;
            if (m_pathfinder->hasReachedNode(this->m_pos))
            {
                // reached destination; can get a part and move on.
                if (m_pathfinder->hasReachedDestination(this->m_pos))
                {
                    m_pos = m_pathfinder->foundPath.back().GetPosition();

                    m_pathfinder->foundPath.pop_back();

                    m_vel.SetZero();

                    b_reachedDestination = true;
                }
                else
                {
                    m_pos = m_pathfinder->foundPath.back().GetPosition();

                    m_pathfinder->foundPath.pop_back();

                    SetVelocity(CheckVelocity(m_pos, m_pathfinder->foundPath.back().GetPosition()));
                    SetDirection(CheckDirection(m_vel));
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
                
            }
        }
    }

    
    switch (m_state)
    {
    case IDLE:
        DoIdle();
        break;

    case REPAIR:
        DoRepair();
        break;

    case REFILL:
        DoRefill();
        break;

    case BREAK:
        DoBreak();
        break;
    }
}

void MaintenanceMan::Sense(double dt)
{
    if ((m_state == REFILL || m_state == REPAIR) && m_doingWork)
        m_timer += dt;
    else if (m_state == BREAK && m_inToilet)
        m_timer += dt;
    else if (m_state == IDLE)
    {
        m_timer += dt;
        if (m_timer > 1)
        {
            m_timer = 0;
            m_breakCharge += Math::RandFloatMinMax(0, 120);
        }
    }
}

int MaintenanceMan::Think()
{
    switch (m_state)
    {

    case IDLE:
    {
        Vector3 temp = m_workstation->GetPos();
        temp.y -= 1;

        // Check if at workstation
        if ((m_pos - temp).Length() < 1.2)
        {
            // Read Messages
            if (b_newMsgNotif)
            {
                Message* retrivedMsg = this->ReadMessageBoard(SharedData::GetInstance()->m_messageBoard);

                // Check if retrieved message is invalid
                if (retrivedMsg)
                {
                    switch (retrivedMsg->GetMessageType())
                    {
                    case Message::MACHINE_BROKEN:
                        m_targetMachine = dynamic_cast<Machine*>(retrivedMsg->GetMessageFromObject());
                        m_targetMachine->SetIsBeingWorkedOn(true);
                        return REPAIR;
                        break;

                    case Message::MACHINE_REFILL:
                        m_targetMachine = dynamic_cast<Machine*>(retrivedMsg->GetMessageFromObject());
                        m_targetMachine->SetIsBeingWorkedOn(true);
                        return REFILL;
                        break;
                    }
                }
            }
        }
                 
        if (m_breakCharge >= 2000)
        {
            randNum = Math::RandIntMinMax(0, 100);
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

        //// Check if at workstation
        //if ((m_pos - temp).Length() < 1.2)
        //{
        //    int value = ScanMachines();
        //    if (value != IDLE)
        //        return value;
        //}

        break;
    }

    case REFILL:
        if (m_targetMachine && !m_targetMachine->IsEmpty())
        {
            m_targetMachine->SetIsBeingWorkedOn(false);
            m_targetMachine = NULL;
            m_doingWork = false;
            return IDLE;
        }
        //else if (m_targetMachine && m_targetMachine->IsEmpty())
        //{
        //    return REFILL;
        //}
            break;

    case REPAIR:
        if (m_targetMachine && !m_targetMachine->IsBroken())
        {
            m_targetMachine->SetIsBeingWorkedOn(false);
            m_targetMachine = NULL;
            m_doingWork = false;
            return IDLE;
        }
        //else if (m_targetMachine && m_targetMachine->IsBroken())
        //{
        //    return REPAIR;
        //}
        break;

    case BREAK:
        if (m_breakDone)
        {
            m_breakDone = false;
            m_breakCharge = 0;
            return MAINTENANCEMAN_STATES_TOTAL;
        }
        else
        {
            m_targetMachine = NULL;
            m_doingWork = false;
        }
        break;

    }

    return -1;
}

void MaintenanceMan::Act(int value)
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
        break;

    case REPAIR:
        SetState(REPAIR);
        b_reachedDestination = false;
        //DoRepair();

        // pathfind to target machine
        m_pathfinder->EmptyPath();
        m_pathfinder->ReceiveCurrentPos(Vector3(RoundOff(m_pos.x), RoundOff(m_pos.y), m_pos.z));
        m_pathfinder->ReceiveDestination(m_targetMachine->GetPos());
        m_pathfinder->FindPathGreedyBestFirst();

        SetVelocity(CheckVelocity(m_pos, m_pathfinder->foundPath.back().GetPosition()));
        SetDirection(CheckDirection(m_vel));
        break;

    case REFILL:
        SetState(REFILL);
        b_reachedDestination = false;
        //DoRefill();

        // pathfind to target machine
        m_pathfinder->EmptyPath();
        m_pathfinder->ReceiveCurrentPos(Vector3(RoundOff(m_pos.x), RoundOff(m_pos.y), m_pos.z));
        m_pathfinder->ReceiveDestination(m_targetMachine->GetPos());
        m_pathfinder->FindPathGreedyBestFirst();

        SetVelocity(CheckVelocity(m_pos, m_pathfinder->foundPath.back().GetPosition()));
        SetDirection(CheckDirection(m_vel));
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
        break;

    case MAINTENANCEMAN_STATES_TOTAL:   // going back to workstation from toilet
        SetState(IDLE);
        b_reachedDestination = false;
        //DoIdle();

        // pathfind to workstation
        m_pathfinder->EmptyPath();
        m_pathfinder->ReceiveCurrentPos(Vector3(14, 14, m_pos.z));
        m_pathfinder->ReceiveDestination(m_origSpawn);
        m_pathfinder->FindPathGreedyBestFirst();

        SetVelocity(CheckVelocity(m_pos, m_pathfinder->foundPath.back().GetPosition()));
        SetDirection(CheckDirection(m_vel));
        break;
    }
}

int MaintenanceMan::ScanMachines()
{
    for (int i = 0; i < SharedData::GetInstance()->m_goList.size(); ++i)
    {
        GameObject* go = dynamic_cast<GameObject*>(SharedData::GetInstance()->m_goList[i]);

        if (!go->IsActive())
            continue;

        if (go->GetName() == "Machine")
        {
            Machine* machine = dynamic_cast<Machine*>(go);

            if (machine->GetIsBeingWorkedOn())
                continue;

            if (machine->GetState() == Machine::BROKEN)
            {
                m_targetMachine = machine;
                m_targetMachine->SetIsBeingWorkedOn(true);
                return REPAIR;
            }
            else if (machine->GetState() == Machine::WAITFORREFILL)
            {
                m_targetMachine = machine;
                m_targetMachine->SetIsBeingWorkedOn(true);
                return REFILL;
            }
        }
    }

    return IDLE;
}

void MaintenanceMan::DoIdle()
{

}

void MaintenanceMan::DoRepair()
{       
    if (m_timer > 4)
    {
        m_targetMachine->SetIsBroken(false);
        m_timer = 0;
    }
}

void MaintenanceMan::DoRefill()
{
    if (m_timer > 4)
    {
        m_targetMachine->SetIsEmpty(false);
        m_targetMachine->Refill();
        m_timer = 0;
    }
}

void MaintenanceMan::DoBreak()
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
 
        m_toilet->SetOccupied(false);
    }
}

MaintenanceMan::MAINTENANCEMAN_STATE MaintenanceMan::GetState()
{
    return m_state;
}

void MaintenanceMan::SetState(MAINTENANCEMAN_STATE state)
{
    this->m_state = state;
    this->SetSprite();
}

int MaintenanceMan::GetStateInt()
{
    return m_state;
}

int MaintenanceMan::GetMaxStates()
{
    return MAINTENANCEMAN_STATES_TOTAL;
}

void MaintenanceMan::SetWorkstation(Workstation* station)
{
    m_workstation = station;
}

void MaintenanceMan::SetToilet(Toilet* toilet)
{
    m_toilet = toilet;
}

Toilet* MaintenanceMan::GetToilet()
{
    return m_toilet;
}

double MaintenanceMan::GetBreakCharge()
{
    return m_breakCharge;
}

Pathfinder* MaintenanceMan::GetPathfinder()
{
    return m_pathfinder;
}

Machine* MaintenanceMan::GetTargetMachine()
{
    return m_targetMachine;
}