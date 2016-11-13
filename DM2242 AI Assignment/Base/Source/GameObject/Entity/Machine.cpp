#include "Machine.h"

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

void Machine::SetPartToCreate(ROBOT_PART part)
{
    m_partToCreate = part;
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
    }

	if (m_state == WAITFORREFILL)
	{
		if (m_scrapQuantity > m_partToCreate)
		{
			return REST;
		}
	}

	if (m_state == BROKEN)
	{
		if (!m_isBroken)
			return PRODUCTION;
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
			break;
		}
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
    switch (m_partToCreate)
    {
    case HEAD:
        m_scrapQuantity -= 2;
        break;

    case BODY:
        m_scrapQuantity -= 4;
        break;

    case LIMB:
        m_scrapQuantity -= 3;
        break;

    case MICROCHIP:
        m_scrapQuantity -= 1;
        break;
    }
}

void Machine::SetIsBroken(bool status)
{
	m_isBroken = status;
}