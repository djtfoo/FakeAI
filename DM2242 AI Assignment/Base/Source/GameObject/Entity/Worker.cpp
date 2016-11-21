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
    m_atWorkstation = false;
    m_inToilet = false;

	m_workstation = NULL;
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


}

void Worker::Sense(double dt)
{
    m_breakCharge += dt * Math::RandFloatMinMax(1, 2);

    if (m_state == WORK || m_state == BREAK)
        m_timer += dt;
}

int Worker::Think()
{
    switch (m_state)
    {

    case IDLE:
        if (m_breakCharge >= 100)
            //return BREAK;
            ;
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
        if (m_inToilet == false)
        {
            m_timer = 0.0;
            return IDLE;
        }
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
        m_state = IDLE;
        DoIdle();
        break;

    case WORK:
        m_state = WORK;
        DoWork();
        break;

    //case BREAK:
    //    m_state = BREAK;
    //    DoBreak();
    //    break;
    }
}

void Worker::DoIdle()
{
    if (m_pos != m_workstation->GetPos())
    {
        // Set bool to move back to workplace
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

            tempRobot->SetMesh(SharedData::GetInstance()->m_meshList->GetMesh(GEO_ROBOT_STAGE1));
            tempRobot->SetRobotState(Robot::INCOMPLETE1);
            tempRobot->SetWaypoint(1);

            SharedData::GetInstance()->m_goList.push_back(tempRobot);
        }
        else
        {
            if (m_workstation->GetCurrRobot())
            {
                m_workstation->GetCurrRobot()->SetRobotState(static_cast<Robot::ROBOT_STATE>(m_workstation->GetCurrRobot()->GetRobotState() + 1));
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
    if (m_timer > 5)
        m_inToilet = false;
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