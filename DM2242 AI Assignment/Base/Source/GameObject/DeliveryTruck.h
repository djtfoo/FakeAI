#ifndef DELIVERYTRUCK_H
#define DELIVERYTRUCK_H

#include "GameObject.h"

class DeliveryTruck : public GameObject
{
    int m_dir;

public:
    DeliveryTruck();
    virtual ~DeliveryTruck();

    virtual void Init();
    virtual void Update(double dt);

    virtual void SetSprite();

    void SetDirection(int dir);
};

#endif