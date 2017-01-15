#ifndef _MESSAGEBOARD_H
#define _MESSAGEBOARD_H

#include "Message.h"
#include <vector>

class MessageBoard
{
    std::vector<Message*> m_messageLog;
    const int maxSize;

    double d_pulseTimer;

public:
    MessageBoard();
    ~MessageBoard();

    void Update(double dt);
    void Exit();

    void AddMessage(Message* message);
    void PopMessage(int idx);
    void PopAnAcknowledgedMessage();    // pops the oldest acknowledged message

    Message* GetAMessage(int idx);
    int GetMessageListSize();
    void SendNotification();
};

#endif