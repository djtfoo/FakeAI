#ifndef TOILET_H
#define TOILET_H

#include "Entity/Worker.h"
#include <queue>

class Toilet : public GameObject
{
public:
    Toilet();
    virtual ~Toilet();

    virtual void Init();
    virtual void Update(double dt); // Check if any robotpart is at workstation, if so add into vector

private:
    
    std::queue<GameObject*> m_ToiletQueue;

};

#endif