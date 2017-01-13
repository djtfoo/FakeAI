#ifndef MESSAGE_H
#define MESSAGE_H

#include <string>
#include "../Clock/Time.h"

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

        MESSAGE_TYPES_TOTAL     // also acts as NIL
    };

private:
    bool b_isAcknowledged;

    MESSAGE_TYPE m_type;

    std::string m_messageTo;
    std::string m_messageFrom;

    Time m_time;

public:
    Message(MESSAGE_TYPE type, std::string msgTo, std::string msgFrom, Time time);
    ~Message();

    std::string GetMessage();
    std::string GetMessageTo();
    std::string GetMessageFrom();
    std::string GetTime();

    bool IsAcknowledged();
    void SetAcknowledged(bool b_acknowledged);
    MESSAGE_TYPE GetMessageType();
};

#endif