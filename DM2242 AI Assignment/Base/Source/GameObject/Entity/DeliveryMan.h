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
        DELIVERYMAN_STATES_TOTAL
    };

private:
    DELIVERYMAN_STATE m_state;

    virtual void Sense(double dt);
    virtual int Think();
    virtual void Act(int value);

public:
    DeliveryMan();
    virtual ~DeliveryMan();

    virtual void Init();
    virtual void Update(double dt);

    DELIVERYMAN_STATE GetState();
    void SetState(DELIVERYMAN_STATE state);
    virtual int GetStateInt();
    virtual int GetMaxStates();
};

#endif