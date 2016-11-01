#include "Machine.h"

Machine::Machine()
{
}

Machine::~Machine()
{
}

void Machine::Sense(double dt)
{

}

int Machine::Think()
{


    return 0;
}

void Machine::Act(int value)
{
    switch (value)
    {
    case 0:
        break;

    case 1:
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