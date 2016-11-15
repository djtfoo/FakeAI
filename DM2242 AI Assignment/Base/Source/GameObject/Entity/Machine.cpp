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
    m_state = REST;
    m_timer = 0;
	m_storedTimer = 0;
	m_overheatTimer = 0;
    m_overheatCharge = 0;
    m_scrapQuantity = 10; 
    m_maxScrapQuantity = 10;
	m_isBroken = false;
	m_isEmpty = false;
	m_partCreated = false;
}

void Machine::Update(double dt)
{
	switch (m_state)
	{
	case REST:
		m_mesh = SharedData::GetInstance()->m_meshList->GetMesh(GEO_MACHINE);
		break;

	case PRODUCTION:
		m_mesh = SharedData::GetInstance()->m_meshList->GetMesh(GEO_MACHINE_PRODUCTION);
		break;

	case BROKEN:
		m_mesh = SharedData::GetInstance()->m_meshList->GetMesh(GEO_MACHINE_BROKEN);
		break;

	case WAITFORREFILL:
		m_mesh = SharedData::GetInstance()->m_meshList->GetMesh(GEO_MACHINE_WAITFORREFILL);
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
		m_overheatCharge += dt;
		m_overheatTimer += dt;
	}
}

int Machine::Think()
{
	// Change to switch case maybe?
	
    if (m_state == REST && m_timer > 5)
    {
        m_timer = 0.0;

        if (m_scrapQuantity < m_partToCreate)
        {
            return WAITFORREFILL;
            m_timer = 0.0;
        }
        else
        {
            return PRODUCTION;
            m_timer = 0.0;
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
			int RandNum = Math::RandIntMinMax(0, 100);
            if (RandNum < m_overheatCharge)
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
            return PRODUCTION;
        }
        else
            return BROKEN;
	}


    return 0;
}

void Machine::Act(int value)
{
    switch (value)
    {
    case REST:
        m_state = REST;
        break;

    case PRODUCTION:
        m_state = PRODUCTION;
		if (m_timer > 5)
		{
			m_partCreated = true;
			CreatePart();
		}
        break;
    case WAITFORREFILL:
        m_state = WAITFORREFILL;
        m_isEmpty = true;
        break;

    case BROKEN:
        m_state = BROKEN;
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
		}
	}

    switch (m_partToCreate)
    {
    case RobotPart::HEAD:
        m_scrapQuantity -= 2;
        tempPart->SetMesh(SharedData::GetInstance()->m_meshList->GetMesh(GEO_ROBOT_HEAD));

        break;

    case RobotPart::BODY:
        m_scrapQuantity -= 4;
        tempPart->SetMesh(SharedData::GetInstance()->m_meshList->GetMesh(GEO_ROBOT_BODY));
		tempPart->SetWaypoint(0);
        break;

    case RobotPart::LIMB:
        m_scrapQuantity -= 3;
        tempPart->SetMesh(SharedData::GetInstance()->m_meshList->GetMesh(GEO_ROBOT_LIMBS));
        break;

    case RobotPart::MICROCHIP:
        m_scrapQuantity -= 1;
        tempPart->SetMesh(SharedData::GetInstance()->m_meshList->GetMesh(GEO_ROBOT_CHIP));
        break;
    }

    SharedData::GetInstance()->m_goList.push_back(tempPart);
}

void Machine::SetIsBroken(bool status)
{
	m_isBroken = status;
}

void Machine::SetIsEmpty(bool status)
{
    m_isEmpty = status;

	// Debug, refill scrap
	m_scrapQuantity = m_maxScrapQuantity;
}

Machine::MACHINE_STATE Machine::GetState()
{
    return m_state;
}