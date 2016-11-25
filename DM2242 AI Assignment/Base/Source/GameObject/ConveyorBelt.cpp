#include "ConveyorBelt.h"

ConveyorBelt::ConveyorBelt() : GameObject("ConveyorBelt")
{}

ConveyorBelt::~ConveyorBelt()
{}

void ConveyorBelt::Init()
{
	// Set Initial pos as first checkpoint
	m_Checkpoints.push_back(m_pos); 
}

void ConveyorBelt::Update(double dt)
{
}

void ConveyorBelt::AddCheckpoint(const Vector3& checkpoint)
{
	m_Checkpoints.push_back(checkpoint);
}

Vector3 ConveyorBelt::GetNextCheckpoint(int currIdx)
{
	if (currIdx < m_Checkpoints.size() - 1)
		return m_Checkpoints[++currIdx];
	else
		return Vector3(0, 0, 0);
}

Vector3 ConveyorBelt::GetCheckpoint(int Idx)
{
	if (Idx < m_Checkpoints.size())
		return m_Checkpoints[Idx];
}