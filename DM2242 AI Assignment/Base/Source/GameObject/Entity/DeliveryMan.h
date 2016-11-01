#ifndef DELIVERY_MAN_H
#define DELIVERY_MAN_H

#include "Entity.h"

class DeliveryMan : public Entity
{
public:
    enum DELIVERYMAN_STATE
    {
        IDLE,
        DRIVING,
        COLLECT_PRODUCT,
    };

private:
    DELIVERYMAN_STATE m_state;

public:
    DeliveryMan();
    virtual ~DeliveryMan();

    virtual void Sense(double dt);
    virtual int Think();
    virtual void Act(int value);
};

#endif