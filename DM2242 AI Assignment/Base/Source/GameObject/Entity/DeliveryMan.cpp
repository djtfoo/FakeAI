#include "DeliveryMan.h"
#include "../../SharedData.h"

DeliveryMan::DeliveryMan() : Entity("Delivery Man")
{
}

DeliveryMan::~DeliveryMan()
{
    if (m_pathfinder)
        delete m_pathfinder;
}

void DeliveryMan::Init()
{
    m_pathfinder = new Pathfinder();

    m_state = IDLE;
    m_dir = DIR_DOWN;
    m_deliveryTruck = 0;

    b_ornamentCompleted = false;
    m_ornamentToCollect = NULL;
    d_timerCounter = 0.0;
}

void DeliveryMan::Update(double dt)
{
    switch (m_state)
    {
    case IDLE:
        break;

    case DRIVING:
        m_pos += m_vel * dt;
        if (m_pathfinder->hasReachedNode(this->m_pos))
        {
            // reached destination; can get a part and move on.
            if (m_pathfinder->hasReachedDestination(this->m_pos))
            {
                m_pathfinder->foundPath.pop_back();

                m_vel.SetZero();
                SetDirection(DIR_UP);
            }
            else
            {
                m_pathfinder->foundPath.pop_back();

                SetVelocity(CheckVelocity(m_pos, m_pathfinder->foundPath.back().GetPosition()));
                SetDirection(CheckDirection(m_vel));

                m_deliveryTruck->SetDirection(this->m_dir);
            }
        }

        m_deliveryTruck->SetPos(m_pos);
        break;

    case WALK:
        break;

    case COLLECT_PRODUCT:
        break;
    }
}

void DeliveryMan::Sense(double dt)
{
    if (SharedData::GetInstance()->m_ornamentSystem->IsJustCompleted())
    {
        b_ornamentCompleted = true;
    }
}

int DeliveryMan::Think()
{
    if (b_ornamentCompleted)
        return DRIVING;

    return -1;
}

void DeliveryMan::Act(int value)
{
    switch (value)
    {
    case IDLE:
        break;

    case DRIVING:
        SetState(DRIVING);
        b_ornamentCompleted = false;
        m_ornamentToCollect = SharedData::GetInstance()->m_ornamentSystem->GetCompletedOrnament();

        // Pathfind to the completed ornament
        m_pathfinder->ReceiveCurrentPos(this->m_pos);
        m_pathfinder->ReceiveDestination(m_ornamentToCollect->GetPos() + Vector3(0, -1, 0));
        m_pathfinder->FindPathGreedyBestFirst();

        SetVelocity(CheckVelocity(m_pos, m_pathfinder->foundPath.back().GetPosition()) );
        SetDirection(CheckDirection(m_vel));

        m_deliveryTruck->SetDirection(this->m_dir);
        break;
        
    case WALK:
        break;

    case COLLECT_PRODUCT:
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

void DeliveryMan::AssignDeliveryTruck(DeliveryTruck* truck)
{
    this->m_deliveryTruck = truck;
}