#ifndef DELIVERYTRUCK_H
#define DELIVERYTRUCK_H

#include "GameObject.h"

class DeliveryTruck : public GameObject
{
    int m_dir;

    bool b_moving;

public:
    DeliveryTruck();
    virtual ~DeliveryTruck();

    virtual void Init();
    virtual void Update(double dt);

    virtual void SetSprite();

    // Setters
    void SetDirection(int dir);
    void SetMoving(bool moving);

    // Getters
    bool IsMoving();
};

#endif