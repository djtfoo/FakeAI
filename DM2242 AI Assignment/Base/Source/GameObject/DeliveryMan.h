#ifndef DELIVERY_MAN_H
#define DELIVERY_MAN_H

#include "Entity.h"

class DeliveryMan : public Entity
{
public:
    DeliveryMan();
    virtual ~DeliveryMan();

    virtual void Sense(double dt)
    {

    }

    virtual int Think()
    {


        return 0;
    }

    virtual void Act(int value)
    {
        switch (value)
        {
        case 0:
            break;

        case 1:
            break;
        }
    }
};

#endif