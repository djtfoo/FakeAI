#ifndef MESSAGEBOARD_H
#define MESSAGEBOARD_H

#include "Message.h"
#include <vector>

class MessageBoard
{
    std::vector<Message*> m_messageLog;

public:
    MessageBoard();
    ~MessageBoard();

    void Exit();

    void AddMessage(Message* message);
    void PopMessage();
};

#endif