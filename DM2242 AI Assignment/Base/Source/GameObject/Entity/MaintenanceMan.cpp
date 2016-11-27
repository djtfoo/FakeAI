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
    m_breakCharge = 0;
    m_timer = 0;
    m_state = IDLE;
    m_targetMachine = NULL;
    m_doingWork = false;
}

void MaintenanceMan::Update(double dt)
{
    if (m_targetMachine)
    {
        if (!m_doingWork)
        {
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
        Vector3 temp = m_workstation->GetPos();
        temp.y -= 1;

        if (temp != m_pos)
        {
            Vector3 dir = (temp - m_pos).Normalized();
            m_pos += dir * dt;
        }
    }
}

void MaintenanceMan::Sense(double dt)
{
    m_breakCharge += dt * Math::RandFloatMinMax(1, 2);

    if ((m_state == REFILL || m_state == REPAIR) && m_doingWork)
        m_timer += dt;
}

int MaintenanceMan::Think()
{
 
    switch (m_state)
    {

    case IDLE:
    {
        Vector3 temp = m_workstation->GetPos();
        temp.y -= 1;

        if ((m_pos - temp).Length() < 1.001)
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