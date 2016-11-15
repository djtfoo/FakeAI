#ifndef ROBOT_PART_H
#define ROBOT_PART_H

#include "GameObject.h"

class RobotPart : public GameObject
{
public:
    enum ROBOT_PART
    {
        HEAD = 2,
        BODY = 4,
        LIMB = 3,
        MICROCHIP = 1,
    };

    RobotPart();
    virtual ~RobotPart();

    virtual void Init();
    virtual void Update(double dt);

    int GetCost();

    void SetType(ROBOT_PART type);
    ROBOT_PART GetType();

private:
    int m_cost;
    ROBOT_PART m_type;
};

#endif
