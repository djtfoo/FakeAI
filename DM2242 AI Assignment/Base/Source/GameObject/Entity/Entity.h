#ifndef ENTITY_H
#define ENTITY_H

#include "../GameObject.h"
#include "../../MessageBoard/MessageBoard.h"

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

    /*Entity* newTempRole;

    newTempRole = NULL;
    //newTempRole = &thisMaintenaceMan;

    newTempRole = new MaintenanceMan();
    newTempRole = CopyDataOverlah;

    if (newTempRole == NULL)
    {
        Worker::Update();   // basically, call my own update
    }
    else
    {
        newTempRole->Update();
        m_pos = newTempRole->GetPosition();
        m_dir = newTempRole->GetDirection();
    }

    regardless of newTempRole,
        we still call Worker::Render();

    but,
        we need his position & direction (& velocity)?

    Render()
    {
        if (newTempRole)
        {
            RenderHatOverlay();
        }
    }*/

    DIRECTION m_dir;
    Vector3 m_vel;

    bool b_newMsgNotif;    // notification that there is a new message on MessageBoard

public:
    virtual ~Entity();
    
    virtual void Init() = 0;
    virtual void Update(double dt) = 0;      // doing things for its state (e.g. walking)

    virtual void RunFSM(double dt);

    // setting Entity's velocity
    Vector3 CheckVelocity(const Vector3& ownPos, const Vector3& destinationPos);    // get velocity based on destination
    Vector3 CheckVelocity(DIRECTION dir);       // get velocity based on direction

    void SetVelocity(const Vector3& velocity);

    // setting Entity's direction
    DIRECTION CheckDirection(const Vector3& velocity);   // check for direction based on velocity
    DIRECTION CheckDirection(const Vector3& ownPos, const Vector3& toFacePos);    // check for direction based on where to face; can be workstation or based on previous position

    void SetDirection(DIRECTION dir);

    // setting Entity's sprite
    virtual void SetSprite();   // edit the mesh TexCoords
    virtual int GetStateInt();     // default just in case
    virtual int GetMaxStates(); // default just in case

    Message::MESSAGE_TYPE ReadMessageBoard(MessageBoard* mb);
    void SetNewMessageNotif(bool b_notif);

    // idea:
    // multiple types of think and act... use int values for switch case
    // Act(Think());
};

#endif