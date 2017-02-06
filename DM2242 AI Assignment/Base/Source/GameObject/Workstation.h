#ifndef WORK_STATION_H
#define WORK_STATION_H

#include "RobotParts.h"
#include <vector>

#include "../SharedData.h"
#include "Entity\Robot.h"

class Workstation : public GameObject
{
public:
    Workstation(std::string name = "Workstation");
    virtual ~Workstation();

    virtual void Init();
    virtual void Update(double dt); // Check if any robotpart is at workstation, if so add into vector

    void AddToStorage(RobotPart* part);
    RobotPart* RemoveFromStorage();

    bool IfHasRobotPart();
    bool IfRobotAtStation();

    void SetTypeStored(RobotPart::ROBOT_PART type);
    RobotPart::ROBOT_PART GetTypeStored();

    Robot* GetCurrRobot();
    void SetCurrRobot(Robot* robot);

    Vector3 GetRobotPrevPos();

    int GetStoredAmount();

private:
    std::vector<RobotPart*> m_RobotPartStorage;
    bool m_robotAtStation;
    RobotPart::ROBOT_PART m_partToStore;
    Robot* m_currRobot;

    std::vector<Robot*> m_robots;   // robots that have arrived at the Workstation

    Vector3 m_robotPrevPos;
};

#endif