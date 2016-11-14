#include "Machine.h"
//#include "../../SceneBase.h"

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
    m_overheatCharge = 0;
    m_scrapQuantity = 10; 
    m_maxScrapQuantity = 10;
	m_isBroken = false;
	m_partCreated = false;
}

void Machine::Update(double dt)
{

}

void Machine::SetPartToCreate(RobotPart::ROBOT_PART part)
{
    m_partToCreate = part;
}

void Machine::SetSpawnLocation(ConveyorBelt* location)
{
    m_SpawnLocation = location;
}

void Machine::Sense(double dt)
{
    m_timer += dt;

    if (m_state == PRODUCTION)
        m_overheatCharge += dt;
}

int Machine::Think()
{
	// Change to switch case maybe?
	
    if (m_state == REST && m_timer > 10)
    {
        m_timer = 0.0;

        if (m_scrapQuantity < m_partToCreate)
        {
            return WAITFORREFILL;
        }
        else
        {
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
		// Run the check if break down every 0.5 secs
		else if (fmod(m_timer, 0.5) == 0)
		{
			int RandNum = Math::RandIntMinMax(0, 100);
			if (RandNum < m_overheatCharge)
				return BROKEN;
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
			return PRODUCTION;
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
    tempPart->SetType(m_partToCreate);
    tempPart->SetPos(m_SpawnLocation->GetPos());
    tempPart->SetActive();

    switch (m_partToCreate)
    {
    case RobotPart::HEAD:
        m_scrapQuantity -= 2;
        break;

    case RobotPart::BODY:
        m_scrapQuantity -= 4;
        break;

    case RobotPart::LIMB:
        m_scrapQuantity -= 3;
        break;

    case RobotPart::MICROCHIP:
        m_scrapQuantity -= 1;
        break;
    }

    // Set Mesh base on m_partToCreate 
    //tempPart->SetMesh(SceneBase::meshList[SceneBase::GEO_ROBOT_HEAD]);
    m_goList.push_back(tempPart);
}

void Machine::SetIsBroken(bool status)
{
	m_isBroken = status;
}