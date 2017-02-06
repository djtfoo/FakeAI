#include "Workstation.h"

Workstation::Workstation(std::string name) : GameObject(name)
{}

Workstation::~Workstation()
{}

void Workstation::Init()
{
    m_robotAtStation = false;
    m_currRobot = NULL;
    m_partToStore = RobotPart::NIL;

    m_robotPrevPos.SetZero();
}

void Workstation::Update(double dt)
{
    if (m_currRobot == NULL && !m_robots.empty())
    {
        m_currRobot = m_robots.back();
        m_robots.pop_back();
    }

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

        if (go->GetName() == "Robot" && go->IsActive())
        {
            if ((m_pos - go->GetPos()).Length() < 1.1f)
            {
                Robot* robot = dynamic_cast<Robot*>(SharedData::GetInstance()->m_goList[i]);

                if (!robot->IsWorkedOn())
                {
                    switch (robot->GetState())
                    {
                    case Robot::INCOMPLETE_1: // Body
                        if (GetTypeStored() == RobotPart::HEAD)
                        {
                            m_robotAtStation = true;
                            //m_currRobot = robot;
                            robot->SetWorkedOn(true);

                            m_robotPrevPos = robot->GetPos();
                            robot->SetPos(Vector3(this->m_pos.x, this->m_pos.y, 1.f));
                            m_robots.push_back(robot);
                        }
                        break;

                    case Robot::INCOMPLETE_2: // Body + Head
                        if (GetTypeStored() == RobotPart::LIMB)
                        {
                            m_robotAtStation = true;
                            //m_currRobot = robot;
                            robot->SetWorkedOn(true);

                            m_robotPrevPos = robot->GetPos();
                            robot->SetPos(Vector3(this->m_pos.x, this->m_pos.y, 1.f));
                            m_robots.push_back(robot);
                        }
                        break;

                    case Robot::INCOMPLETE_3: // Body + Head + Limbs
                        if (GetTypeStored() == RobotPart::MICROCHIP)
                        {
                            m_robotAtStation = true;
                            //m_currRobot = robot;
                            robot->SetWorkedOn(true);

                            m_robotPrevPos = robot->GetPos();
                            robot->SetPos(Vector3(this->m_pos.x, this->m_pos.y, 1.f));
                            m_robots.push_back(robot);
                        }
                        break;
                    }
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
    if (m_currRobot == NULL)
        return false;

    return true;

    //return m_robotAtStation;
}

Robot* Workstation::GetCurrRobot()
{
    return m_currRobot;
}

void Workstation::SetCurrRobot(Robot* robot)
{
    m_currRobot = robot;
}

Vector3 Workstation::GetRobotPrevPos()
{
    return m_robotPrevPos;
}

int Workstation::GetStoredAmount()
{
    return m_RobotPartStorage.size();
}