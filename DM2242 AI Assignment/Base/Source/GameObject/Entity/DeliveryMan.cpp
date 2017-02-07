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
    b_reachedDestination = false;

    m_state = IDLE;
    m_dir = DIR_DOWN;
    m_deliveryTruck = 0;

    d_timerCounter = 0.0;

    m_ornamentToCollect = NULL;
    //for (int i = 0; i < 3; ++i)
    //   m_completedOrnaments[i] = NULL;
}

void DeliveryMan::Update(double dt)
{
    // Update message notification
    if (b_newMsgNotif)
    {
        UpdateMessageNotif(dt);
    }
    if (b_renderAcknowledgeMsg)
    {
        UpdateMessageAcknowledged(dt);
    }

    //if (SharedData::GetInstance()->m_ornamentSystem->IsJustCompleted())
    //{
    //    AddCompletedOrnament(SharedData::GetInstance()->m_ornamentSystem->GetCompletedOrnament());
    //}

    switch (m_state)
    {
    case IDLE:
        if (!m_pathfinder->IsPathEmpty())
        {
            m_pos += m_vel * dt;
            m_deliveryTruck->SetMoving(true);

            if (m_pathfinder->hasReachedNode(this->m_pos))
            {
                // reached destination; can get a part and move on.
                if (m_pathfinder->hasReachedDestination(this->m_pos))
                {
                    WhenReachedDestination();
                    //b_reachedDestination = false;
                    m_deliveryTruck->SetMoving(false);
                }
                else
                {
                    WhenReachedPathNode();
                    m_deliveryTruck->SetDirection(this->m_dir);
                }
            }

            m_deliveryTruck->SetPos(m_pos);
        }

        break;

    case DRIVING:
        m_pos += m_vel * dt;
        m_deliveryTruck->SetMoving(true);

        if (m_pathfinder->hasReachedNode(this->m_pos))
        {
            // reached destination; can get a part and move on.
            if (m_pathfinder->hasReachedDestination(this->m_pos))
            {
                WhenReachedDestination();
                m_deliveryTruck->SetMoving(false);
            }
            else
            {
                WhenReachedPathNode();
                m_deliveryTruck->SetDirection(this->m_dir);
            }
        }

        m_deliveryTruck->SetPos(m_pos);
        break;

    case WALK:
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
        break;

    case COLLECT_PRODUCT:
        m_pos += m_vel * dt;
        if (m_pathfinder->hasReachedNode(this->m_pos))
        {
            // reached destination; can get a part and move on.
            if (m_pathfinder->hasReachedDestination(this->m_pos))
            {
                WhenReachedDestination();
                //m_ornamentToCollect = GetOrnamentToCollect();   // if there is no more, it will return 0
            }
            else
            {
                WhenReachedPathNode();
            }
        }
        break;
    }
}

void DeliveryMan::Sense(double dt)
{
    //if (m_state == IDLE && m_vel.IsZero() && m_ornamentToCollect == NULL)
    //{
    //    m_ornamentToCollect = GetOrnamentToCollect();
    //}
}

int DeliveryMan::Think()
{
    if (m_state == IDLE)
    {
        // Read Messages
        if (b_newMsgNotif && d_msgNotifTimer >= 2.0)
        {
            Message* retrievedMsg = this->ReadMessageBoard(SharedData::GetInstance()->m_messageBoard);

            // Check if retrieved message is invalid
            if (retrievedMsg)
            {
                AcknowledgeMessage();
                m_ornamentToCollect = dynamic_cast<Ornament*>(retrievedMsg->GetMessageFromObject());
                return DRIVING;
            }
        }
    }

    if (b_reachedDestination && m_state == DRIVING)
        return WALK;

    if (b_reachedDestination && m_state == WALK)
        return COLLECT_PRODUCT;

    if (b_reachedDestination && m_state == COLLECT_PRODUCT)
    {
        // Read Messages
        if (b_newMsgNotif)
        {
            Message* retrievedMsg = this->ReadMessageBoard(SharedData::GetInstance()->m_messageBoard);

            // Check if retrieved message is invalid
            if (retrievedMsg)
            {
                AcknowledgeMessage();
                m_ornamentToCollect = dynamic_cast<Ornament*>(retrievedMsg->GetMessageFromObject());
                return WALK;
            }
        }

        return IDLE;
    }

    return -1;
}

void DeliveryMan::Act(int value)
{
    switch (value)
    {
    case IDLE:
        SetState(IDLE);

        // Pathfind to outside the screen
        m_pathfinder->ReceiveCurrentPos(Vector3(RoundOff(m_pos.x), RoundOff(m_pos.y), m_pos.z));
        m_pathfinder->ReceiveDestination(Vector3(0, Math::RandIntMinMax(0, 3), 0));
        m_pathfinder->FindPathGreedyBestFirst();

        SetVelocity(CheckVelocity(m_pos, m_pathfinder->foundPath.back().GetPosition()));
        SetDirection(CheckDirection(m_vel));
        m_pathfinder->ReceiveDirection(m_dir);

        m_deliveryTruck->SetDirection(this->m_dir);

        break;

    case DRIVING:
        SetState(DRIVING);
        b_reachedDestination = false;

        // Pathfind to coordinate near the ornaments
        m_pathfinder->ReceiveCurrentPos(Vector3(RoundOff(m_pos.x), RoundOff(m_pos.y), m_pos.z));
        m_pathfinder->ReceiveDestination(Vector3(4.f, 1.f, 0));
        m_pathfinder->FindPathGreedyBestFirst();

        SetVelocity(CheckVelocity(m_pos, m_pathfinder->foundPath.back().GetPosition()) );
        SetDirection(CheckDirection(m_vel));
        m_pathfinder->ReceiveDirection(m_dir);

        m_deliveryTruck->SetDirection(this->m_dir);
        break;
        
    case WALK:
        b_reachedDestination = false;
        SetState(WALK);

        m_deliveryTruck->SetDirection(DIR_UP);

        // Pathfind to the completed ornament
        m_pathfinder->ReceiveCurrentPos(Vector3(RoundOff(m_pos.x), RoundOff(m_pos.y), m_pos.z));
        m_pathfinder->ReceiveDestination(m_ornamentToCollect->GetPos() + Vector3(0, -1, 0));
        m_pathfinder->FindPathGreedyBestFirst();

        SetVelocity(CheckVelocity(m_pos, m_pathfinder->foundPath.back().GetPosition()));
        SetDirection(CheckDirection(m_vel));
        m_pathfinder->ReceiveDirection(m_dir);
        break;

    case COLLECT_PRODUCT:
    {
                            b_reachedDestination = false;
                            SetState(COLLECT_PRODUCT);

                            m_ornamentToCollect->RemoveOrnament();

                            // Pathfind to the delivery truck
                            m_pathfinder->ReceiveCurrentPos(this->m_pos);
                            Vector3 pos = m_deliveryTruck->GetPos();
                            m_pathfinder->ReceiveDestination(Vector3(RoundOff(pos.x), RoundOff(pos.y), pos.z));
                            m_pathfinder->FindPathGreedyBestFirst();

                            SetVelocity(CheckVelocity(m_pos, m_pathfinder->foundPath.back().GetPosition()));
                            SetDirection(CheckDirection(m_vel));
                            m_pathfinder->ReceiveDirection(m_dir);
                            break;
    }

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

//void DeliveryMan::AddCompletedOrnament(Ornament* ornament)
//{
//    for (int i = 0; i < 3; ++i)
//    {
//        if (m_completedOrnaments[i] == NULL)
//        {
//            m_completedOrnaments[i] = ornament;
//            break;
//        }
//    }
//}

//Ornament* DeliveryMan::GetOrnamentToCollect()
//{
//    for (int i = 0; i < 3; ++i)
//    {
//        if (m_completedOrnaments[i])
//        {
//            Ornament* ornament = m_completedOrnaments[i];
//            m_completedOrnaments[i] = 0;
//            return ornament;
//        }
//    }
//
//    return NULL;    // no ornament to collect
//}

//Pathfinder* DeliveryMan::GetPathfinder()
//{
//    return m_pathfinder;
//}