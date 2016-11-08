#include "Worker.h"

Worker::Worker() : Entity("Worker")
{
}

Worker::~Worker()
{
}

void Worker::Init()
{

}

void Worker::Update(double dt)
{

}

void Worker::Sense(double dt)
{

}

int Worker::Think()
{


    return 0;
}

void Worker::Act(int value)
{
    switch (value)
    {
    case 0:
        break;

    case 1:
        break;
    }
}

bool Worker::IsPartAtWorkstation()
{
    return true;
}

bool Worker::IsOnBreak()
{
    if (m_state == BREAK)
        return true;

    return false;
}