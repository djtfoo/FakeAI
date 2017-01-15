#include "Worker.h"

#include "Robot.h"

Worker::Worker() : Entity("Worker")
{
}

Worker::~Worker()
{
    if (m_pathfinder)
        delete m_pathfinder;
}

void Worker::Init()
{
    m_pathfinder = new Pathfinder();
    b_reachedDestination = false;

    m_state = IDLE;
    m_timer = 0;
    m_breakCharge = Math::RandFloatMinMax(-1000, 1000);
    m_workCompleted = false;
    m_atWorkstation = true;
    m_breakDone = false;
    m_inToilet = false;
    m_doOnce = false;
    m_shouldMoveForward = true;
    m_toiletIdx = 0;

	m_workstation = NULL;
    m_toilet = NULL;

    randNum = 0;
}

void Worker::SetPos(Vector3 pos)
{
    m_pos = pos;
    m_origSpawn = m_pos;
}

void Worker::Update(double dt)
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


    switch (m_state)
    {
    case IDLE:
        DoIdle();
        break;

    case WORK:
        DoWork();
        break;

    case BREAK:
        DoBreak();
        break;
    }
}

void Worker::Sense(double dt)
{
    if (m_state == WORK)
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

int Worker::Think()
{
    switch (m_state)
    {

    case IDLE:
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
        else if (IsAbleToWork())
            return WORK;

        break;

    case WORK:

        if (m_workCompleted)
        {
            m_timer = 0.0;
            m_workCompleted = false;
            return IDLE;
        }
        break;

    case BREAK:
        if (m_breakDone)
        {
            m_breakDone = false;
            m_breakCharge = 0;
            return IDLE;
        }
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
        b_reachedDestination = false;
        //DoIdle();

        // pathfind to workstation
        m_pathfinder->EmptyPath();
        m_pathfinder->ReceiveCurrentPos(Vector3(14, 14, m_pos.z));
        m_pathfinder->ReceiveDestination(m_origSpawn);
        m_pathfinder->FindPathGreedyBestFirst();

        SetVelocity(CheckVelocity(m_pos, m_pathfinder->foundPath.back().GetPosition()));
        SetDirection(CheckDirection(m_vel));
        m_pathfinder->ReceiveDirection(m_dir);
        break;

    case WORK:
        SetState(WORK);
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

    if (m_atWorkstation)
        SetDirection(DIR_DOWN);
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
                m_workstation->GetCurrRobot()->SetPos(m_workstation->GetRobotPrevPos());
                //m_workstation->GetCurrRobot()->SetVelocity(CheckVelocity(m_pos, m_workstation->GetCurrRobot()->GetWaypoint()));
                m_workstation->SetCurrRobot(NULL);
            }
        }

        m_timer = 0;
        m_workstation->RemoveFromStorage();
    }
}

void Worker::DoBreak()
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

double Worker::GetBreakCharge()
{
    return m_breakCharge;
}

Pathfinder* Worker::GetPathfinder()
{
    return m_pathfinder;
}