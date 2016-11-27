#include "Worker.h"

#include "Robot.h"

Worker::Worker() : Entity("Worker")
{
}

Worker::~Worker()
{
}

void Worker::Init()
{
    m_state = IDLE;
    m_timer = 0;
	m_breakCharge = 0;
    m_workCompleted = false;
    m_atWorkstation = true;
    m_breakDone = false;
    m_inToilet = false;
    m_doOnce = false;
    m_toiletIdx = 0;

	m_workstation = NULL;
}

void Worker::SetPos(Vector3 pos)
{
    m_pos = pos;
    m_origSpawn = m_pos;
}

void Worker::Update(double dt)
{
	switch (m_state)
	{
	case IDLE:
		//m_mesh = SharedData::GetInstance()->m_meshList->GetMesh(GEO_WORKER);
		break;

	case WORK:
		//m_mesh = SharedData::GetInstance()->m_meshList->GetMesh(GEO_WORKER_WORKING);
		break;
	}

    if (m_state == BREAK)
    {
        Vector3 dir = (m_toilet->GetQueuePosition(m_toiletIdx) - m_pos).Normalized();
        m_pos += dir * dt;

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
        if (m_origSpawn != m_pos)
        {
            Vector3 dir = (m_origSpawn - m_pos).Normalized();
            m_pos += dir * dt;

            if ((m_pos - m_origSpawn).Length() < 0.1)
                m_atWorkstation = true;
        }
    }
}

void Worker::Sense(double dt)
{
    if (m_state == WORK)
        m_timer += dt;

    else if (m_state == BREAK && m_inToilet)
        m_timer += dt;
    else
    {
        m_timer += dt;
        if (m_timer > 1)
        {
            m_timer = 0;
            m_breakCharge += Math::RandFloatMinMax(0, 100);
        }
    }
}

int Worker::Think()
{
    switch (m_state)
    {

    case IDLE:
        if (m_breakCharge >= 1000)
            return BREAK;     
        else if (IsAbleToWork())
            return WORK;
        else
            return IDLE;

        break;

    case WORK:

        if (m_workCompleted)
        {
            m_timer = 0.0;
            m_workCompleted = false;
            return IDLE;
        }
        else
            return WORK;
        break;

    case BREAK:
        if (m_breakDone)
            return IDLE;
        else
            return BREAK;
        break;
      
    
    }

    return -1;
}

void Worker::Act(int value)
{
    switch (value)
    {
    case IDLE:
        SetState(IDLE);
        DoIdle();
        break;

    case WORK:
        SetState(WORK);
        DoWork();
        break;

    case BREAK:
        SetState(BREAK);
        DoBreak();
        break;
    }
}

void Worker::DoIdle()
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
}

void Worker::DoWork()
{
    if (m_timer > 4)
    {
        m_workCompleted = true;

        if (m_workstation->GetTypeStored() == RobotPart::BODY)
        {

            Robot* tempRobot = new Robot();
            tempRobot->Init();
            tempRobot->SetActive();

            tempRobot->SetPos(m_pos - Vector3(0, 1, 0));

            for (int i = 0; i < SharedData::GetInstance()->m_goList.size(); ++i)
            {
                if (SharedData::GetInstance()->m_goList[i]->GetName() == "ConveyorBelt")
                {
                    tempRobot->SetBelt(dynamic_cast<ConveyorBelt*>(SharedData::GetInstance()->m_goList[i]));
                }
            }

            tempRobot->SetMesh(SharedData::GetInstance()->m_meshList->GetMesh(GEO_ROBOT));
            tempRobot->SetState(Robot::INCOMPLETE_1);
            tempRobot->SetWaypoint(1);

            SharedData::GetInstance()->m_goList.push_back(tempRobot);
        }
        else
        {
            if (m_workstation->GetCurrRobot())
            {
                m_workstation->GetCurrRobot()->SetState(static_cast<Robot::ROBOT_STATE>(m_workstation->GetCurrRobot()->GetState() + 1));
                m_workstation->GetCurrRobot()->SetWorkedOn(false);
                m_workstation->SetCurrRobot(NULL);
            }
        }

        m_timer = 0;
        m_workstation->RemoveFromStorage();
    }
}

void Worker::DoBreak()
{
    if (!m_doOnce)
    {
        m_toiletIdx = m_toilet->AddToQueue(this);
        m_doOnce = true;
    }

    if (m_toilet->CheckIfChange())
        m_toiletIdx--;

    if (m_timer > 4)
    {
        m_breakCharge = 0;
        m_breakDone = true;
        m_timer = 0;
        m_toilet->RemoveFromQueue();
        m_doOnce = false;
        m_toilet->SetOccupied(false);
    }
}

bool Worker::IsPartAtWorkstation()
{
    if (m_workstation->IfHasRobotPart())
        return true;

    return false;
}

bool Worker::IsOnBreak()
{
    if (m_state == BREAK)
        return true;

    return false;
}


void Worker::SetWorkstation(Workstation* station)
{
    m_workstation = station;
}

bool Worker::IsAbleToWork()
{
    if (!m_atWorkstation)
        return false;

    switch (m_workstation->GetTypeStored())
    {
    case RobotPart::BODY:
        if (IsPartAtWorkstation())
            return true;
        break;
    case RobotPart::HEAD:
        if (IsPartAtWorkstation() && m_workstation->IfRobotAtStation())
            return true;
        break;
    case RobotPart::LIMB:
        if (IsPartAtWorkstation() && m_workstation->IfRobotAtStation())
            return true;
        break;
    case RobotPart::MICROCHIP:
        if (IsPartAtWorkstation() && m_workstation->IfRobotAtStation())
            return true;
        break;
    }

    return false;
}

Worker::WORKER_STATE Worker::GetState()
{
    return m_state;
}

void Worker::SetState(WORKER_STATE state)
{
    this->m_state = state;
    this->SetSprite();
}

int Worker::GetStateInt()
{
    return m_state;
}

int Worker::GetMaxStates()
{
    return WORKER_STATES_TOTAL;
}

void Worker::SetToilet(Toilet* toilet)
{
    m_toilet = toilet;
}

Toilet* Worker::GetToilet()
{
    return m_toilet;
}