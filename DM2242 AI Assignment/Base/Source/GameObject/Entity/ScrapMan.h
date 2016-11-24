#ifndef SCRAP_MAN_H
#define SCRAP_MAN_H

#include "Entity.h"

class ScrapMan : public Entity
{
public:
    enum SCRAPMAN_STATE
    {
        IDLE,
        PICKUP_ROBOT,
        //BREAKDOWN_ROBOT,
        BREAK,
        SCRAPMAN_STATES_TOTAL
    };

private:
    double d_breakCharge;

    SCRAPMAN_STATE m_state;

    virtual void Sense(double dt);
    virtual int Think();
    virtual void Act(int value);

public:
    ScrapMan();
    virtual ~ScrapMan();

    virtual void Init();
    virtual void Update(double dt);

    SCRAPMAN_STATE GetState();
    void SetState(SCRAPMAN_STATE state);
    virtual int GetStateInt();
    virtual int GetMaxStates();
};

#endif