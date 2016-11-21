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
        }

        if ((m_pos - m_targetMachine->GetPos()).Length() < 1.001)
        {
            m_doingWork = true;
        }
    }
}

void MaintenanceMan::Sense(double dt)
{
    m_breakCharge += dt * Math::RandFloatMinMax(1, 2);

    if ((m_state == REFILL || m_state == REPAIR) && m_doingWork)
        m_timer += dt;
    else if (m_state == REPAIR)
        m_timer += dt;
}

int MaintenanceMan::Think()
{
 
    switch (m_state)
    {

    case IDLE:
        return ScanMachines();
        break;
        
    case REFILL:
        break;

    case REPAIR:
        break;

    case BREAK:
        break;

    }

    return 0;
}

void MaintenanceMan::Act(int value)
{
    switch (value)
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

            if (machine->GetState() == Machine::BROKEN)
            {
                m_targetMachine = machine;
                return REPAIR;
            }
            else if (machine->GetState() == Machine::WAITFORREFILL)
            {
                m_targetMachine = machine;
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
        m_timer = 0;
    }
}

void MaintenanceMan::DoBreak()
{

}