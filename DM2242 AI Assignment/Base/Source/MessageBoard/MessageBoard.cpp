#include "MessageBoard.h"

MessageBoard::MessageBoard()
{

}

MessageBoard::~MessageBoard()
{
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