#ifndef TOILET_H
#define TOILET_H

#include "Entity/Worker.h"
#include <queue>

class Toilet : public GameObject
{
public:
    Toilet();
    ~Toilet();

private:
    
    std::queue<GameObject*> m_ToiletQueue;

};

#endif