#ifndef ENTITY_H
#define ENTITY_H

#include "../GameObject.h"

class Entity : public GameObject
{
public:
    enum DIRECTION
    {
        DIR_DOWN,
        DIR_UP,
        DIR_RIGHT,
        DIR_LEFT,
        DIRECTIONS_TOTAL
    };

private:
    virtual void Sense(double dt) = 0;  // get/receive updates from the world
    virtual int Think() = 0;   // process the updates
    virtual void Act(int value) = 0;     // act upon any change in behaviour

protected:
    Entity(std::string name);

    DIRECTION m_dir;

public:
    virtual ~Entity();
    
    virtual void Init() = 0;
    virtual void Update(double dt) = 0;      // doing things for its state (e.g. walking)

    virtual void RunFSM(double dt);

    // setting Entity's direction
    //DIRECTION CheckDirection(const Vector3& velocity);   // check for direction based on velocity
    //DIRECTION CheckDirection(const Vector3& ownPos, const Vector3& toFacePos);    // check for direction based on where to face; can be workstation or based on previous position
    void SetDirection(DIRECTION dir);

    // setting Entity's sprite
    virtual void SetSprite();   // edit the mesh TexCoords
    virtual int GetStateInt();     // default just in case
    virtual int GetMaxStates(); // default just in case

    // idea:
    // multiple types of think and act... use int values for switch case
    // Act(Think());
};

#endif