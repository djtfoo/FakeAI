#include "MaintenanceMan.h"

#include "../../SharedData.h"

MaintenanceMan::MaintenanceMan() : Entity("Maintenance Man")
{
}

MaintenanceMan::~MaintenanceMan()
{
}

void MaintenanceMan::Init()
{
    m_breakCharge = Math::RandFloatMinMax(-1000, 1000);
    m_timer = 0;
    m_state = IDLE;
    m_targetMachine = NULL;
    m_doingWork = false;
    m_breakDone = false;
    m_inToilet = false;
    m_doOnce = false;
    m_toiletIdx = -1;
    
    m_toilet = NULL;
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
    }
    else if (m_targetMachine)
    {
        if (!m_doingWork)
        {
            // Move Towards TargetMachine, then do work
            Vector3 dir = (m_targetMachine->GetPos() - m_pos).Normalized();
            m_pos += dir * dt;

            if ((m_pos - m_targetMachine->GetPos()).Length() < 1.001)
            {
                m_doingWork = true;
            }
        }
    }
    else
    {
        // Move to workstation if no TargetMachine
        if ((m_origSpawn - m_pos).Length() > 0.1)
        {
            Vector3 dir = (m_origSpawn - m_pos);

            if (!dir.IsZero())
                dir.Normalize();

            m_pos += dir * dt;

            if ((m_pos - m_origSpawn).Length() < 0.1)
            {
                
            }
        }
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
            m_breakCharge += Math::RandFloatMinMax(0, 200);
        }
    }
}

int MaintenanceMan::Think()
{
 
    switch (m_state)
    {

    case IDLE:
    {
        if (m_breakCharge >= 2000)
        {
            m_doOnce = false;
            return BREAK;
        }
     
        Vector3 temp = m_workstation->GetPos();
        temp.y -= 1;

        // Check if at workstation
        if ((m_pos - temp).Length() < 1.2)
        {
            return ScanMachines();
        }
        else
        {
            return IDLE;
        }
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
        else if (m_targetMachine && m_targetMachine->IsEmpty())
        {
            return REFILL;
        }
            break;

    case REPAIR:
        if (m_targetMachine && !m_targetMachine->IsBroken())
        {
            m_targetMachine->SetIsBeingWorkedOn(false);
            m_targetMachine = NULL;
            m_doingWork = false;
            return IDLE;
        }
        else if (m_targetMachine && m_targetMachine->IsBroken())
        {
            return REPAIR;
        }
        break;

    case BREAK:
        if (m_breakDone)
        {
            m_breakDone = false;
            m_breakCharge = 0;
            return IDLE;
        }
        else
        {
            m_targetMachine = NULL;
            m_doingWork = false;
            return BREAK;
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
        DoIdle();
        break;

    case REPAIR:
        SetState(REPAIR);
        DoRepair();
        break;

    case REFILL:
        SetState(REFILL);
        DoRefill();
        break;

    case BREAK:
        SetState(BREAK);
        DoBreak();
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
        m_toiletIdx = Math::Max(--m_toiletIdx, 0);

    if (m_timer > 4)
    {
        m_breakCharge = 0;
        m_breakDone = true;
        m_timer = 0;
        m_toilet->RemoveFromQueue();
        std::cout << "POPPED" << std::endl;
 
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