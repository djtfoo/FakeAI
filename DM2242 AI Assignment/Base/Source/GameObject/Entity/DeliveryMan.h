#ifndef DELIVERY_MAN_H
#define DELIVERY_MAN_H

#include "Entity.h"
#include "Pathfinder.h"
#include "../DeliveryTruck.h"
#include "../Ornament.h"

class DeliveryMan : public Entity
{
public:
    enum DELIVERYMAN_STATE
    {
        IDLE,
        DRIVING,
        WALK,
        COLLECT_PRODUCT,

        DELIVERYMAN_STATES_TOTAL
    };

private:
    DELIVERYMAN_STATE m_state;

    DeliveryTruck* m_deliveryTruck;
    Ornament* m_completedOrnaments[3];
    Ornament* m_ornamentToCollect;

    bool b_reachedDestination;
    
    double d_timerCounter;
    Pathfinder* m_pathfinder;

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

    void AssignDeliveryTruck(DeliveryTruck* truck);

    void AddCompletedOrnament(Ornament* ornament);
    Ornament* GetOrnamentToCollect();

    // Getters
    Pathfinder* GetPathfinder();
};

#endif