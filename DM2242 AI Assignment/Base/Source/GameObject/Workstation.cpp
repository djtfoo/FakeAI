#include "Workstation.h"

Workstation::Workstation() : GameObject("Workstation")
{}

Workstation::~Workstation()
{}

void Workstation::Init()
{
}

void Workstation::Update(double dt)
{
    for (int i = 0; i < SharedData::GetInstance()->m_goList.size(); ++i)
    {
        if ((m_pos - SharedData::GetInstance()->m_goList[i]->GetPos()).Length() < 1.001)
        {
			if (SharedData::GetInstance()->m_goList[i]->GetName() == "RobotPart" && SharedData::GetInstance()->m_goList[i]->IsActive())
            {
                SharedData::GetInstance()->m_goList[i]->SetInactive();
                AddToStorage(dynamic_cast<RobotPart*>(SharedData::GetInstance()->m_goList[i]));
				break;
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
