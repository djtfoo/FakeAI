#include "ScrapMan.h"

ScrapMan::ScrapMan() : Entity("Scrap Man")
{
}

ScrapMan::~ScrapMan()
{
}

void ScrapMan::Init()
{
    m_dir = DIR_DOWN;

    m_state = IDLE;
}

void ScrapMan::Update(double dt)
{

}

void ScrapMan::Sense(double dt)
{

}

int ScrapMan::Think()
{


    return -1;
}

void ScrapMan::Act(int value)
{
    switch (value)
    {
    case 0:
        break;

    case 1:
        break;
    }
}

ScrapMan::SCRAPMAN_STATE ScrapMan::GetState()
{
    return m_state;
}

void ScrapMan::SetState(SCRAPMAN_STATE state)
{
    this->m_state = state;
    this->SetSprite();
}

int ScrapMan::GetStateInt()
{
    return m_state;
}

int ScrapMan::GetMaxStates()
{
    return SCRAPMAN_STATES_TOTAL;
}