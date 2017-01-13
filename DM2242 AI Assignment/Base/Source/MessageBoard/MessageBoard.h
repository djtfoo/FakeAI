#ifndef MESSAGEBOARD_H
#define MESSAGEBOARD_H

#include "Message.h"
#include <vector>

class MessageBoard
{
    std::vector<Message*> m_messageLog;
    const int maxSize;

public:
    MessageBoard();
    ~MessageBoard();

    void Exit();

    void AddMessage(Message* message);
    void PopMessage();

    Message* GetMessage(int idx);
    int GetMessageListSize();
};

#endif