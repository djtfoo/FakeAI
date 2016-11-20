#include "Workstation.h"

Workstation::Workstation() : GameObject("Workstation")
{}

Workstation::~Workstation()
{}

void Workstation::Init()
{
    m_robotAtStation = false;
    m_currRobot = NULL;
    m_partToStore = RobotPart::HEAD;
}

void Workstation::Update(double dt)
{
    for (int i = 0; i < SharedData::GetInstance()->m_goList.size(); ++i)
    {
        GameObject* go = dynamic_cast<GameObject*>(SharedData::GetInstance()->m_goList[i]);
        if ((m_pos - go->GetPos()).Length() < 1.001)
        {
            if (go->GetName() == "RobotPart" && go->IsActive())
            {
                RobotPart* part = dynamic_cast<RobotPart*>(go);
                if (part->GetType() == m_partToStore)
                {
                    part->SetInactive();
                    AddToStorage(dynamic_cast<RobotPart*>(part));
                }
            }
        }

        if ((m_pos - go->GetPos()).Length() < 1.05)
        {
            if (go->GetName() == "Robot" && go->IsActive())
            {
                Robot* robot = dynamic_cast<Robot*>(SharedData::GetInstance()->m_goList[i]);
                switch (robot->GetRobotState())
                {
                case Robot::INCOMPLETE1: // Body
                    if (GetTypeStored() == RobotPart::HEAD)
                    {
                        m_robotAtStation = true;
                        m_currRobot = robot;
                        robot->SetWorkedOn(true);
                    }
                    break;

                case Robot::INCOMPLETE2: // Body + Head
                    if (GetTypeStored() == RobotPart::LIMB)
                    {
                        m_robotAtStation = true;
                        m_currRobot = robot;
                        robot->SetWorkedOn(true);
                    }
                    break;

                case Robot::INCOMPLETE3: // Body + Head + Limbs
                    if (GetTypeStored() == RobotPart::MICROCHIP)
                    {
                        m_robotAtStation = true;
                        m_currRobot = robot;
                        robot->SetWorkedOn(true);
                    }
                    break;
                }
            }
        }
    }
}

void Workstation::AddToStorage(RobotPart* part)
{
    m_RobotPartStorage.push_back(part);
}

RobotPart* Workstation::RemoveFromStorage()
{
    int i = m_RobotPartStorage.size() - 1;

    RobotPart* temp = m_RobotPartStorage[i];
    m_RobotPartStorage.pop_back();
    return temp;
    
}

bool Workstation::IfHasRobotPart()
{
    if (m_RobotPartStorage.empty())
        return false;
    else
        return true;
}

void Workstation::SetTypeStored(RobotPart::ROBOT_PART type)
{
    m_partToStore = type;
}

RobotPart::ROBOT_PART Workstation::GetTypeStored()
{
    return m_partToStore;
}

bool Workstation::IfRobotAtStation()
{
    return m_robotAtStation;
}

Robot* Workstation::GetCurrRobot()
{
    return m_currRobot;
}

void Workstation::SetCurrRobot(Robot* robot)
{
    m_currRobot = robot;
}