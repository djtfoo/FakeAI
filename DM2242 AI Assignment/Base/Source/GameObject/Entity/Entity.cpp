#include "GL\glew.h"

#include "Entity.h"
#include "../../Utility.h"

#include "../../MessageBoard/MessageBoard.h"

#include "../../SharedData.h"

#include "MaintenanceMan.h"
#include "Worker.h"
#include "ScrapMan.h"

bool Entity::b_someoneOnLeave = false;

Entity::Entity(std::string name) : GameObject(name, true)
, m_dir(DIR_DOWN), m_vel(0, 0, 0)
, b_newMsgNotif(false), d_msgNotifTimer(0.0)
, f_symbolTranslation(0.f)
, b_renderMessageComeIn(false), b_renderAcknowledgeMsg(false)
, d_inactive_level(10.0)
, b_urgencyChanged(false)
, b_onLeave(false)
, b_doneTempJob(false)
{
    tempRole = NULL;
    f_walkSpeed = 1;
    i_currUrgencyLevel = 1;
}

Entity::~Entity()
{
    if (m_mesh)
    {
        delete m_mesh;
        m_mesh = NULL;
    }

    if (tempRole)
    {
        delete tempRole;
        tempRole = NULL;
    }
}

bool Entity::IsSomeoneOnLeave()
{
    return b_someoneOnLeave;
}

void Entity::SetSomeoneOnLeave(const bool onLeave)
{
    b_someoneOnLeave = onLeave;
}

void Entity::RunFSM(double dt)
{
    if (tempRole)
    {
        tempRole->RunFSM(dt);
        return;
    }

    Sense(dt);

    int thinkValue = Think();

    if (thinkValue != -1)
    {
        Act(thinkValue);
        //SetSprite();
    }
}

void Entity::SetVelocity(const Vector3& velocity)
{
    this->m_vel = velocity;
}

void Entity::SetDirection(DIRECTION dir)
{
    // if velocity == (1, 0, 0)
    // this->m_dir = DIR_RIGHT;

    this->m_dir = dir;
    this->SetSprite();
}
DIRECTION Entity::GetDirection()
{
    return m_dir;
}

void Entity::SetSprite()
{
    if (this->m_mesh->textureID > 0)
    {
        float texCountWidth = m_dir;
        float texCountHeight = GetMaxStates() - 1 - GetStateInt();
        float texWidth = 1.f / 4;
        float texHeight = 1.f / GetMaxStates();

        TexCoord texCoords[4] = {
            TexCoord(texWidth * texCountWidth, texHeight * texCountHeight),
            TexCoord(texWidth * (texCountWidth + 1), texHeight * texCountHeight),
            TexCoord(texWidth * (texCountWidth + 1), texHeight * (texCountHeight + 1) - 0.01f),
            TexCoord(texWidth * texCountWidth, texHeight * (texCountHeight + 1) - 0.01f)
        };

        glBindBuffer(GL_ARRAY_BUFFER, m_mesh->vertexBuffer);
        for (unsigned i = 0; i < 4; ++i) {
            glBufferSubData(GL_ARRAY_BUFFER, (sizeof(Vertex)-sizeof(TexCoord)) + (i * sizeof(Vertex)), sizeof(TexCoord), &texCoords[i]);
        }
    }
}

int Entity::GetStateInt()
{
    return 0;
}

int Entity::GetMaxStates()
{
    return 1;
}

Vector3 Entity::CheckVelocity(const Vector3& ownPos, const Vector3& destinationPos)
{
    Vector3 vel = (destinationPos - ownPos);
    if (vel.IsZero())
        return vel;
    
    vel.Normalize();
    vel.x = SmallRoundOff(vel.x);
    vel.y = SmallRoundOff(vel.y);

    return vel;
}

Vector3 Entity::CheckVelocity(DIRECTION dir)
{
    if (dir == DIR_DOWN)
        return Vector3(0, -1, 0);

    if (dir == DIR_UP)
        return Vector3(0, 1, 0);

    if (dir == DIR_RIGHT)
        return Vector3(1, 0, 0);

    if (dir == DIR_LEFT)
        return Vector3(-1, 0, 0);

    return Vector3(0, -1, 0);
}

DIRECTION Entity::CheckDirection(const Vector3& velocity)
{
    if (m_vel.IsZero())     // not moving
        return m_dir;    // return previous direction

    if (m_vel.x == 1)
        return DIR_RIGHT;

    if (m_vel.x == -1)
        return DIR_LEFT;

    if (m_vel.y == 1)
        return DIR_UP;

    if (m_vel.y == -1)
        return DIR_DOWN;

    return DIR_DOWN;
}

DIRECTION Entity::CheckDirection(const Vector3& ownPos, const Vector3& toFacePos)
{
    Vector3 tempVel = CheckVelocity(ownPos, toFacePos);

    if (tempVel.IsZero())     // not moving
        return m_dir;    // return previous direction

    if (tempVel.x == 1)
        return DIR_RIGHT;

    if (tempVel.x == -1)
        return DIR_LEFT;

    if (tempVel.y == 1)
        return DIR_UP;

    if (tempVel.y == -1)
        return DIR_DOWN;

    return DIR_DOWN;
}

Message* Entity::ReadMessageBoard(MessageBoard* mb)
{
    if (IsOnLeave())
        return NULL;

    b_newMsgNotif = false;     // messages will be processed now

    int maxSize = mb->GetMessageListSize();

    for (int i = 0; i < maxSize; ++i)  // read the message log from oldest message
    {
        Message* msg = mb->GetAMessage(i);
        //if (msg->GetMessageTo() == this->GetName() && !msg->IsAcknowledged())
        //{
        //    msg->SetAcknowledged(true);
        //    return msg;
        //}

        // Check if acknowledged
        if (msg->IsAcknowledged())
            continue;

        // Check message, if meant for many different entities
        if (msg->GetMessageTo() == "Humans")
        {
            if (this->GetName() == "Worker" || this->GetName() == "Maintenance Man" || this->GetName() == "Scrap Man")
            {
                // Workers, MM and SM shouldn't acknowledge the urgency messages or other supervisor messages so others can also see it
                if (msg->GetMessageType() != Message::INCREASE_URGENCY && msg->GetMessageType() != Message::DECREASE_URGENCY && msg->GetMessageType() != Message::COMPLETED_URGENCY_CHANGE)
                    msg->SetAcknowledged(true);
                else
                    msg->SetAcknowledged(false);

                // Return msg if b_UrgencyChanged and Messagetype match, so that can react to message
                if (msg->GetMessageType() == Message::COMPLETED_URGENCY_CHANGE && this->GetUrgencyChanged() == true)
                    return msg;
                else if ((msg->GetMessageType() == Message::INCREASE_URGENCY || msg->GetMessageType() == Message::DECREASE_URGENCY) && this->GetUrgencyChanged() == false)
                    return msg;
                else
                    int i = 0;
                
            }
            else if (this->GetName() == "Supervisor")
            {
                msg->SetAcknowledged(true);
                return msg;
            }
        }

        // Default message reading
        if (msg->GetMessageTo() == this->GetName())
        {
            msg->SetAcknowledged(true);
            return msg;
        }
    }

    return NULL;
}

void Entity::SetNewMessageNotif(bool b_notif)
{
    if (tempRole)
        tempRole->b_newMsgNotif = b_notif;
    else
        b_newMsgNotif = b_notif;
}

void Entity::ReceiveMessageNotif()
{
    if (tempRole) {
        tempRole->b_renderMessageComeIn = true;
        tempRole->d_msgNotifTimer = 0.0;
        tempRole->f_symbolTranslation = 0.f;
    }
    else {
        b_renderMessageComeIn = true;
        d_msgNotifTimer = 0.0;
        f_symbolTranslation = 0.f;
    }
}

void Entity::UpdateMessageNotif(const double dt)
{
    d_msgNotifTimer += dt;
    if (d_msgNotifTimer >= 1.5)
        b_renderMessageComeIn = false;
    
    UpdateSymbolTranslation(dt);
}

void Entity::UpdateMessageAcknowledged(const double dt)
{
    d_msgNotifTimer += dt;
    if (d_msgNotifTimer >= 1.5)
        b_renderAcknowledgeMsg = false;

    UpdateSymbolTranslation(dt);
}

void Entity::UpdateSymbolTranslation(const double dt)
{
    if (f_symbolTranslation < 0.3f)
    {
        f_symbolTranslation += 0.5f * (float)dt;
        f_symbolTranslation = Math::Min(f_symbolTranslation, 0.3f);
    }
}

bool Entity::HasMessageComeIn()
{
    return b_renderMessageComeIn;
}

float Entity::GetRenderNotifOffset()
{
    return f_symbolTranslation;
}

bool Entity::HasAcknowledgedMessage()
{
    return b_renderAcknowledgeMsg;
}

void Entity::AcknowledgeMessage()
{
    b_renderAcknowledgeMsg = true;
    d_msgNotifTimer = 0.0;
    f_symbolTranslation = 0.f;

    // pop message if the message board is full
    MessageBoard* mb = SharedData::GetInstance()->m_messageBoard;
    if (mb->GetMessageListSize() > mb->GetMaxMessageListSize())
    {
        for (int i = 0; i < mb->GetMessageListSize(); ++i)   // oldest message to latest message
        {
            Message* msg = mb->GetAMessage(i);
            if (msg->IsAcknowledged())
            {
                mb->PopMessage(i);
                break;
            }
        }
    }
}

void Entity::WhenReachedDestination()
{
    m_pos = m_pathfinder->foundPath.back().GetPosition();

    m_pathfinder->foundPath.pop_back();

    m_vel.SetZero();

    b_reachedDestination = true;
}

void Entity::WhenReachedPathNode()
{
    m_pos = m_pathfinder->foundPath.back().GetPosition();

    m_pathfinder->foundPath.pop_back();

    SetVelocity(CheckVelocity(m_pos, m_pathfinder->foundPath.back().GetPosition()));
    SetDirection(CheckDirection(m_vel));

    m_pathfinder->ReceiveDirection(m_dir);
}

bool Entity::HasReachedDestination()
{
    return b_reachedDestination;
}

void Entity::SetTempRole(Entity* newRole)
{
    if (tempRole) {
        delete tempRole;
        tempRole = NULL;
    }

    if (newRole->GetName() == "Maintenance Man") {
        tempRole = new MaintenanceMan();
        tempRole->Init();
        tempRole->SetMesh(SharedData::GetInstance()->m_meshList->GetMesh(GEO_TEMPMESH));
        tempRole->SetPos(this->m_pos);
        tempRole->SetDirection(this->m_dir);

        MaintenanceMan* replacer = dynamic_cast<MaintenanceMan*>(tempRole);
        MaintenanceMan* replaced = dynamic_cast<MaintenanceMan*>(newRole);

        if (this->GetName() == "Worker")
        {
            Worker* thisGuy = dynamic_cast<Worker*>(this);
            replacer->SetWorkstation(thisGuy->GetWorkstation());
        }
        else if (this->GetName() == "Scrap Man")
        {
            ScrapMan* thisGuy = dynamic_cast<ScrapMan*>(this);
            replacer->SetWorkstation(thisGuy->GetScrapPile());
        }

        replacer->SetToilet(replaced->GetToilet());
        //replacer->SetPos(this->m_pos);
        replacer->SetOriginalSpawn(this->m_pos);
    }

    else if (newRole->GetName() == "Scrap Man") {
        tempRole = new ScrapMan();
        tempRole->Init();
        tempRole->SetMesh(SharedData::GetInstance()->m_meshList->GetMesh(GEO_TEMPMESH));
        tempRole->SetPos(this->m_pos);
        tempRole->SetDirection(this->m_dir);

        ScrapMan* replacer = dynamic_cast<ScrapMan*>(tempRole);
        ScrapMan* replaced = dynamic_cast<ScrapMan*>(newRole);

        replacer->SetToilet(replaced->GetToilet());
        replacer->AssignScrapPile(replaced->GetScrapPile());
        replacer->SetOriginalSpawn(replaced->GetOriginalSpawn());


        // pathfind to workstation
        replacer->m_pathfinder->EmptyPath();
        replacer->m_pathfinder->ReceiveCurrentPos(Vector3(RoundOff(m_pos.x), RoundOff(m_pos.y), m_pos.z));
        replacer->m_pathfinder->ReceiveDestination(replacer->GetOriginalSpawn());
        replacer->m_pathfinder->FindPathGreedyBestFirst();

        replacer->SetVelocity(CheckVelocity(m_pos, replacer->m_pathfinder->foundPath.back().GetPosition()));
        replacer->SetDirection(CheckDirection(replacer->m_vel));
        replacer->m_pathfinder->ReceiveDirection(replacer->m_dir);
    }

    else if (newRole->GetName() == "Worker") {
        tempRole = new Worker();
        tempRole->Init();
        tempRole->SetMesh(SharedData::GetInstance()->m_meshList->GetMesh(GEO_TEMPMESH));
        //tempRole->SetPos(this->m_pos);
        tempRole->SetDirection(this->m_dir);

        Worker* replacer = dynamic_cast<Worker*>(tempRole);
        Worker* replaced = dynamic_cast<Worker*>(newRole);

        replacer->SetToilet(replaced->GetToilet());
        replacer->SetWorkstation(replaced->GetWorkstation());
        replacer->SetOriginalSpawn(replaced->GetOriginalSpawn());
    }
}

Entity* Entity::GetTempRole()
{
    return tempRole;
}

void Entity::RemoveTempRole()
{
    if (tempRole)
    {
        delete tempRole;
        tempRole = NULL;
    }

    if (this->GetName() == "Maintenance Man")
    {
        MaintenanceMan* man = dynamic_cast<MaintenanceMan*>(this);
        // pathfind to workstation
        m_pathfinder->EmptyPath();
        m_pathfinder->ReceiveCurrentPos(Vector3(RoundOff(m_pos.x), RoundOff(m_pos.y), m_pos.z));
        m_pathfinder->ReceiveDestination(man->GetOriginalSpawn());
        m_pathfinder->FindPathGreedyBestFirst();

        SetVelocity(CheckVelocity(m_pos, m_pathfinder->foundPath.back().GetPosition()));
        SetDirection(CheckDirection(m_vel));
        m_pathfinder->ReceiveDirection(m_dir);
    }
    else if (this->GetName() == "Scrap Man")
    {
        ScrapMan* man = dynamic_cast<ScrapMan*>(this);
        // pathfind to workstation
        m_pathfinder->EmptyPath();
        m_pathfinder->ReceiveCurrentPos(Vector3(RoundOff(m_pos.x), RoundOff(m_pos.y), m_pos.z));
        m_pathfinder->ReceiveDestination(man->GetOriginalSpawn());
        m_pathfinder->FindPathGreedyBestFirst();

        SetVelocity(CheckVelocity(m_pos, m_pathfinder->foundPath.back().GetPosition()));
        SetDirection(CheckDirection(m_vel));
        m_pathfinder->ReceiveDirection(m_dir);
    }
    else if (this->GetName() == "Worker")
    {
        Worker* worker = dynamic_cast<Worker*>(this);
        // pathfind to workstation
        m_pathfinder->EmptyPath();
        m_pathfinder->ReceiveCurrentPos(Vector3(RoundOff(m_pos.x), RoundOff(m_pos.y), m_pos.z));
        m_pathfinder->ReceiveDestination(worker->GetOriginalSpawn());
        m_pathfinder->FindPathGreedyBestFirst();

        SetVelocity(CheckVelocity(m_pos, m_pathfinder->foundPath.back().GetPosition()));
        SetDirection(CheckDirection(m_vel));
        m_pathfinder->ReceiveDirection(m_dir);
    }
}

int Entity::GetInactiveLevel()
{
    return d_inactive_level;
}

bool Entity::GetUrgencyChanged()
{
    return b_urgencyChanged;
}

bool Entity::IsOnLeave()
{
    return b_onLeave;
}

void Entity::SetOnLeave(const bool onLeave)
{
    b_onLeave = onLeave;
}

Pathfinder* Entity::GetPathfinder()
{
    return m_pathfinder;
}