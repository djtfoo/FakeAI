#ifndef WORK_STATION_H
#define WORK_STATION_H

#include "RobotParts.h"
#include <vector>

#include "../SharedData.h"

class Workstation : public GameObject
{
public: 
    Workstation();
    virtual ~Workstation();

    virtual void Init();
    virtual void Update(double dt); // Check if any robotpart is at workstation, if so add into vector

    void AddToStorage(RobotPart* part);
    RobotPart* RemoveFromStorage();

    bool IfHasRobotPart();

private:
    std::vector<RobotPart*> m_RobotPartStorage;
};

#endif