#ifndef WORK_STATION_H
#define WORK_STATION_H

#include "RobotParts.h"
#include <vector>

class Workstation : public GameObject
{
public: 
    Workstation();
    ~Workstation();

    virtual void Init();
    virtual void Update(double dt); // Check if any robotpart is at workstation, if so add into vector

    void AddToStorage(RobotPart* part);

    bool IfHasRobotPart();

private:
    std::vector<RobotPart*> m_RobotPartStorage;
};

#endif