#include "DeliveryMan.h"

DeliveryMan::DeliveryMan() : Entity("Delivery Man")
{
}

DeliveryMan::~DeliveryMan()
{
}

void DeliveryMan::Init()
{

}

void DeliveryMan::Update(double dt)
{

}

void DeliveryMan::Sense(double dt)
{

}

int DeliveryMan::Think()
{


    return -1;
}

void DeliveryMan::Act(int value)
{
    switch (value)
    {
    case 0:
        break;

    case 1:
        break;
    }
}

DeliveryMan::DELIVERYMAN_STATE DeliveryMan::GetState()
{
    return m_state;
}

void DeliveryMan::SetState(DELIVERYMAN_STATE state)
{
    this->m_state = state;
    this->SetSprite();
}

int DeliveryMan::GetStateInt()
{
    return m_state;
}

int DeliveryMan::GetMaxStates()
{
    return DELIVERYMAN_STATES_TOTAL;
}