#include "MessageBoard.h"
#include "../SharedData.h"
#include "../GameObject/Entity/Entity.h"

MessageBoard::MessageBoard() : maxSize(5)
{
}

MessageBoard::~MessageBoard()
{
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

void MessageBoard::PopMessage()
{
    if (!m_messageLog.empty())
    {
        Message* toPop = *(m_messageLog.begin());
        delete toPop;
        m_messageLog.erase(m_messageLog.begin());
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