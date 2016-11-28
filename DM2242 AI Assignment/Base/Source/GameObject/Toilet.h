#ifndef TOILET_H
#define TOILET_H

#include "GameObject.h"
#include <queue>
#include <vector>

class Toilet : public GameObject
{
    std::queue<GameObject*> m_ToiletQueue;
    std::vector<GameObject*> m_ToiletVec;

    bool b_occupied;
    bool m_change;

public:
    Toilet();
    virtual ~Toilet();

    virtual void Init();
    virtual void Update(double dt); // Check if person in the toilet is done with his business

    virtual void SetSprite();   // edit the mesh TexCoords

    void SetOccupied(bool status);
    bool IsOccupied();      // gets whether someone is in the toilet

    Vector3 GetQueuePosition(int idx);
    int GetToiletSize();

    int AddToQueue(GameObject* object);
    void RemoveFromQueue();

    bool CheckIfChange();
};

#endif