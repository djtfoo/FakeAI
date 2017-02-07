#include "Message.h"

Message::Message(MESSAGE_TYPE type, std::string msgTo, GameObject* msgFrom, Time time, Entity* m_messageSubject)
: m_type(type)
, m_messageTo(msgTo)
, m_messageFrom(msgFrom)
, m_time(time)
, m_messageSubject(m_messageSubject)
{
    b_isAcknowledged = false;
}

Message::Message(MESSAGE_TYPE type, std::string msgTo, std::string msgFrom, Time time, Entity* m_messageSubject)
: m_type(type)
, m_messageTo(msgTo)
, m_messageFromStr(msgFrom)
, m_time(time)
, m_messageFrom(0)
, m_messageSubject(m_messageSubject)
{
    b_isAcknowledged = false;
}

Message::~Message()
{
}

std::string Message::GetMessageString()
{
    switch (m_type)
    {
    case MACHINE_BROKEN: return "I am broken.";
    case MACHINE_REFILL: return "I need refills.";
    case ROBOT_SHUTDOWN: return "I have shut down.";
    case ORNAMENT_COMPLETE: return "An ornament is completed.";
    case ENTITY_NOTATWORK: return "I am going on leave today.";
    case ENTITY_ROLECHANGE: return "Replace " + m_messageSubject->GetName() + ".";
    case MAINTENANCEMAN_NEEDSCRAP: return "I need more scrap parts.";
    case INCREASE_URGENCY: return "Increase urgency.";
    case DECREASE_URGENCY: return "Decrease urgency.";
    case COMPLETED_URGENCY_CHANGE: return "Go back to normal productivity.";
    }

    return "";
}

std::string Message::GetMessageTo()
{
    return m_messageTo;
}

std::string Message::GetMessageFrom()
{
    // Check if an entity has been set
    if (m_messageFrom)
        return m_messageFrom->GetName();
    else
        return m_messageFromStr;
}

GameObject* Message::GetMessageFromObject()
{
    return m_messageFrom;
}

std::string Message::GetTime()
{
    return m_time.GetDayAbbreviation() + " " + m_time.GetTime();
}

bool Message::IsAcknowledged()
{
    return b_isAcknowledged;
}

void Message::SetAcknowledged(bool b_acknowledged)
{
    b_isAcknowledged = b_acknowledged;
}

Message::MESSAGE_TYPE Message::GetMessageType()
{
    return m_type;
}

Entity* Message::GetMessageSubject()
{
    return m_messageSubject;
}