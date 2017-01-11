#include "MessageBoard.h"

MessageBoard::MessageBoard()
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