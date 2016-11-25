#ifndef CONVEYOR_BELT_H
#define CONVEYOR_BELT_H

#include "GameObject.h"
#include <vector>

class ConveyorBelt : public GameObject
{
public:
    ConveyorBelt();
    virtual ~ConveyorBelt();

    virtual void Init();
    virtual void Update(double dt);

	void AddCheckpoint(const Vector3& checkpoint);

	Vector3 GetCheckpoint(int Idx);
	Vector3 GetNextCheckpoint(int currIdx);

    std::vector<Vector3> m_Checkpoints; // Checkpoints to go to
private:
	
};

#endif