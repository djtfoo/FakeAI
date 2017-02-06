#ifndef _MESSAGE_H
#define _MESSAGE_H

#include <string>
#include "../Clock/Time.h"
#include "../GameObject/Entity/Entity.h"

class Message
{
public:
    enum MESSAGE_TYPE
    {
        MACHINE_BROKEN,
        MACHINE_REFILL,
        ROBOT_SHUTDOWN,
        ORNAMENT_COMPLETE,
        ENTITY_NOTATWORK,
        ENTITY_ROLECHANGE,
        MAINTENANCEMAN_NEEDSCRAP,
        INCREASE_URGENCY,
        DECREASE_URGENCY,
        COMPLETED_URGENCY_CHANGE,

        MESSAGE_TYPES_TOTAL     // also acts as NIL
    };

private:
    bool b_isAcknowledged;

    MESSAGE_TYPE m_type;
    Entity* m_messageSubject;

    std::string m_messageTo;
    GameObject* m_messageFrom;
    // String for non-entity or non-gameobject objects
    std::string m_messageFromStr;

    Time m_time;

public:
    Message(MESSAGE_TYPE type, std::string msgTo, GameObject* m_messageFrom, Time time, Entity* subject = NULL);
    Message(MESSAGE_TYPE type, std::string msgTo, std::string m_messageFrom, Time time, Entity* subject = NULL);
    ~Message();

    std::string GetMessageString();
    std::string GetMessageTo();
    std::string GetMessageFrom();
    GameObject* GetMessageFromObject();
    Entity* GetMessageSubject();
    std::string GetTime();

    bool IsAcknowledged();
    void SetAcknowledged(bool b_acknowledged);
    MESSAGE_TYPE GetMessageType();
};

#endif