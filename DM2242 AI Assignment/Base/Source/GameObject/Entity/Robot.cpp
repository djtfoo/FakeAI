#include "Robot.h"

Robot::Robot() : Entity("Robot")
{
}

Robot::~Robot()
{
}

void Robot::Init()
{

}

void Robot::Update(double dt)
{

}

void Robot::Sense(double dt)
{
    m_lifetime += dt;
}

int Robot::Think()
{
    if (m_lifetime >= 120.0)
        return 1;
    else if (1)
        return 2;

    return 0;
}

void Robot::Act(int value)
{
    switch (value)
    {
    case 0:
        //ActShutDown();
        break;

    case 1:
        //ActCheer();
        break;
    }
}