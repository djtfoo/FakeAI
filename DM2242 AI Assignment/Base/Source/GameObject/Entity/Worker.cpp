#include "Worker.h"

Worker::Worker() : Entity("Worker")
{
}

Worker::~Worker()
{
}

void Worker::Init()
{
    m_workCompleted = false;
    m_atWorkstation = false;
    m_inToilet = false;
}

void Worker::Update(double dt)
{

}

void Worker::Sense(double dt)
{
    m_breakCharge += dt * Math::RandFloatMinMax(1, 2);

    if (m_state == WORK || m_state == BREAK)
        m_timer += dt;
}

int Worker::Think()
{
    switch (m_state)
    {

    case IDLE:
        if (m_breakCharge >= 100)
            return BREAK;
        else if (IsPartAtWorkstation())
            return WORK;

        break;

    case WORK:
        if (m_workCompleted)
        {
            m_timer = 0.0;
            m_workCompleted = false;
            return IDLE;
        }
        break;

    case BREAK:
        if (m_inToilet = false)
            m_timer = 0.0;
            return IDLE;
        break;
      
    
    }

    return 0;
}

void Worker::Act(int value)
{
    switch (value)
    {
    case IDLE:
        DoIdle();
        break;

    case WORK:
        DoWork();
        break;

    case BREAK:
        DoBreak();
        break;
    }
}

void Worker::DoIdle()
{
    if (m_pos != m_workstation->GetPos())
    {
        // Set bool to move back to workplace
        m_atWorkstation = false;
    }
}

void Worker::DoWork()
{
    if (m_timer > 4)
        m_workCompleted = true;
}

void Worker::DoBreak()
{
    if (m_timer > 5)
        m_inToilet = false;
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