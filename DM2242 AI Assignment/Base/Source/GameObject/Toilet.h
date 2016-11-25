#ifndef TOILET_H
#define TOILET_H

#include "Entity/Worker.h"
#include <queue>

class Toilet : public GameObject
{
    std::queue<GameObject*> m_ToiletQueue;

    bool b_occupied;

public:
    Toilet();
    virtual ~Toilet();

    virtual void Init();
    virtual void Update(double dt); // Check if person in the toilet is done with his business

    virtual void SetSprite();   // edit the mesh TexCoords

    bool IsOccupied();      // gets whether someone is in the toilet
};

#endif