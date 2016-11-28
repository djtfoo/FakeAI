#include "Machine.h"
#include "../../SharedData.h"

Machine::Machine() : Entity("Machine")
{
}

Machine::~Machine()
{
}

void Machine::Init()
{
    m_dir = DIR_DOWN;

    m_state = REST;
    m_timer = (double)(Math::RandFloatMinMax(0.f, 2.f));  // init at diff times so machines will start production at random times
	m_storedTimer = 0;
	m_overheatTimer = 0;
    m_overheatCharge = 0;
    m_scrapQuantity = 10; 
    m_maxScrapQuantity = 10;
	m_isBroken = false;
	m_isEmpty = false;
	m_partCreated = false;
    m_isBeingWorkedOn = false;

    randNum = 0;
}

void Machine::Update(double dt)
{
	switch (m_state)
	{
	case REST:
		//m_mesh = SharedData::GetInstance()->m_meshList->GetMesh(GEO_MACHINE);
		break;

	case PRODUCTION:
		//m_mesh = SharedData::GetInstance()->m_meshList->GetMesh(GEO_MACHINE_PRODUCTION);
		break;

	case BROKEN:
		//m_mesh = SharedData::GetInstance()->m_meshList->GetMesh(GEO_MACHINE_BROKEN);
		break;

	case WAITFORREFILL:
		//m_mesh = SharedData::GetInstance()->m_meshList->GetMesh(GEO_MACHINE_WAITFORREFILL);
		break;
	}
}

void Machine::SetPartToCreate(RobotPart::ROBOT_PART part)
{
    m_partToCreate = part;
}

void Machine::SetSpawnLocation(const Vector3& location)
{
    m_SpawnLocation = location;
}

void Machine::Sense(double dt)
{
	if (m_state != BROKEN && m_state != WAITFORREFILL)
		m_timer += dt;

	if (m_state == PRODUCTION)
	{
		m_overheatCharge += dt * 0.85;
		m_overheatTimer += dt;
	}
}

int Machine::Think()
{
    //~~~~~~~~~~~~~~~~
	// Change to switch case maybe?
    //~~~~~~~~~~~~~~~~

    if (m_state == REST && m_timer > 5)
    {
        if (m_scrapQuantity < m_partToCreate)
        {
            return WAITFORREFILL;
        }
        else
        {
            m_timer = 0.0;
            return PRODUCTION;
        }
    }

    if (m_state == PRODUCTION)
    {
        if (m_partCreated)
        {
            m_timer = 0.0;
            m_partCreated = false;
            return REST;
        }
		// Run the check if break down every 1.0 sec
		//else if (int(fmod(m_timer, 1.0)) == 0)
		else if (m_overheatTimer > 0.5)
		{
			randNum = Math::RandIntMinMax(10, 100);
            if (randNum < m_overheatCharge)
            {
                m_storedTimer = m_timer;
                return BROKEN;
            }
			m_overheatTimer = 0.0;
		}
        else
            return PRODUCTION;

    }

	if (m_state == WAITFORREFILL)
	{
        if (m_scrapQuantity > m_partToCreate)
            return REST;
        else
            return WAITFORREFILL;
	}

	if (m_state == BROKEN)
	{
        if (!m_isBroken)
        {
            m_overheatCharge = 0.0;
            m_timer = m_storedTimer;
            m_storedTimer = 0.0;
            return PRODUCTION;
        }
        else
            return BROKEN;
	}

    return -1;
}

void Machine::Act(int value)
{
    switch (value)
    {
    case REST:
        SetState(REST);
        break;

    case PRODUCTION:
        SetState(PRODUCTION);
		if (m_timer > 5)
		{
			m_partCreated = true;
            m_timer = 0.0;
			CreatePart();
		}
        break;
    case WAITFORREFILL:
        SetState(WAITFORREFILL);
        m_isEmpty = true;
        break;

    case BROKEN:
        SetState(BROKEN);
        m_isBroken = true;
        break;
    }
}

double Machine::GetTimer()
{
    return m_timer;
}

double Machine::GetOverheatCharge()
{
    return m_overheatCharge;
}

int Machine::GetScrapQuantity()
{
    return m_scrapQuantity;
}

bool Machine::IsBroken()
{
    if (m_state == BROKEN)
        return true;

    return false;
}

bool Machine::IsEmpty()
{
    if (m_state == WAITFORREFILL)
        return true;

    return false;
}

void Machine::CreatePart()
{
    RobotPart* tempPart = new RobotPart();
	tempPart->Init();
    tempPart->SetType(m_partToCreate);
    tempPart->SetPos(m_SpawnLocation);
    tempPart->SetActive();

	for (int i = 0; i < SharedData::GetInstance()->m_goList.size(); ++i)
	{
		if (SharedData::GetInstance()->m_goList[i]->GetName() == "ConveyorBelt")
		{
			tempPart->SetBelt(dynamic_cast<ConveyorBelt*>(SharedData::GetInstance()->m_goList[i]));
            break;
		}
	}

    switch (m_partToCreate)
    {
    case RobotPart::HEAD:
        m_scrapQuantity -= 2;
        //tempPart->SetMesh(SharedData::GetInstance()->m_meshList->GetMesh(GEO_ROBOT_HEAD));
        tempPart->SetWaypoint(2);
        break;

    case RobotPart::BODY:
        m_scrapQuantity -= 4;
        //tempPart->SetMesh(SharedData::GetInstance()->m_meshList->GetMesh(GEO_ROBOT_BODY));
        tempPart->SetWaypoint(0);
        break;

    case RobotPart::LIMB:
        m_scrapQuantity -= 3;
        //tempPart->SetMesh(SharedData::GetInstance()->m_meshList->GetMesh(GEO_ROBOT_LIMBS));
        tempPart->SetWaypoint(4);
        break;

    case RobotPart::MICROCHIP:
        m_scrapQuantity -= 1;
        //tempPart->SetMesh(SharedData::GetInstance()->m_meshList->GetMesh(GEO_ROBOT_CHIP));
        tempPart->SetWaypoint(6);
        break;
    }

    tempPart->SetMesh(SharedData::GetInstance()->m_meshList->GetMesh(GEO_ROBOT_PART_PACKAGE));

    SharedData::GetInstance()->m_goList.push_back(tempPart);
}

void Machine::SetIsBroken(bool status)
{
	m_isBroken = status;
}

void Machine::SetIsEmpty(bool status)
{
    m_isEmpty = status;
}

Machine::MACHINE_STATE Machine::GetState()
{
    return m_state;
}

void Machine::SetState(MACHINE_STATE state)
{
    this->m_state = state;
    this->SetSprite();
}

int Machine::GetStateInt()
{
    return m_state;
}

int Machine::GetMaxStates()
{
    return MACHINE_STATES_TOTAL;
}

void Machine::Refill()
{
    m_scrapQuantity = m_maxScrapQuantity; 
}

void Machine::SetIsBeingWorkedOn(bool status)
{
    m_isBeingWorkedOn = status;
}

bool Machine::GetIsBeingWorkedOn()
{
    return m_isBeingWorkedOn;
}

int Machine::GetMaxScrapQuantity()
{
    return m_maxScrapQuantity;
}

int Machine::GetPartToCreate()
{
    return (int)m_partToCreate;
}