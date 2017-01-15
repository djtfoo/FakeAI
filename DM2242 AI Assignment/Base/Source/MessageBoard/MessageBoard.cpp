#include "MessageBoard.h"
#include "../SharedData.h"
#include "../GameObject/Entity/Entity.h"

MessageBoard::MessageBoard() : maxSize(5), d_pulseTimer(0.0)
{
}

MessageBoard::~MessageBoard()
{
}

void MessageBoard::Update(double dt)
{
    // Periodically pulse notifications
    d_pulseTimer += dt;

    if (d_pulseTimer > 1.0)
    {
        bool msgNotAcknowledged = false;

        // Check if msg acknowledged
        int maxSize = this->GetMessageListSize();

        for (int i = 0; i < maxSize; ++i) 
        {
            Message* msg = this->GetAMessage(i);
            if (!msg->IsAcknowledged())
            {
                msgNotAcknowledged = true;
                break;
            }
        }

        if (msgNotAcknowledged)
        {
            SendNotification();
            d_pulseTimer = 0.0;
        }
    }
}

void MessageBoard::Exit()
{
    while (m_messageLog.size() > 0)
    {
        Message* msg = m_messageLog.back();
        delete msg;
        m_messageLog.pop_back();
    }
}

void MessageBoard::AddMessage(Message* message)
{
    m_messageLog.push_back(message);

    SendNotification();
}

void MessageBoard::PopMessage()
{
    if (!m_messageLog.empty())
    {
        Message* toPop = *(m_messageLog.begin());
        delete toPop;
        m_messageLog.erase(m_messageLog.begin());
    }
}

void MessageBoard::SendNotification()
{
    // notify every Entity about the new message
    for (int i = 0; i < SharedData::GetInstance()->m_goList.size(); ++i)
    {
        GameObject* go = SharedData::GetInstance()->m_goList[i];
        if (go->IsEntity())
        {
            Entity* entity = dynamic_cast<Entity*>(go);
            entity->SetNewMessageNotif(true);
        }
    }
}

Message* MessageBoard::GetAMessage(int idx)
{
    return m_messageLog[idx];
}

int MessageBoard::GetMessageListSize()
{
    return m_messageLog.size();
}