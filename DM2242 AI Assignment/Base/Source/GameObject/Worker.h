#ifndef WORKER_H
#define WORKER_H

#include "Entity.h"

class Worker : public Entity
{
    double d_breakCharge;

public:
    Worker();
    virtual ~Worker();

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