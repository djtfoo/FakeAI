#ifndef ORNAMENT_H
#define ORNAMENT_H

#include "GameObject.h"

class Ornament : public GameObject
{
public:
    enum ORNAMENT_STAGE     // stages of completion
    {
        STAGE_1,
        STAGE_2,
        STAGE_3,
        STAGE_COMPLETE,

        STAGES_TOTAL
    };

private:

    int m_partCount;    // current number of parts
    ORNAMENT_STAGE m_stage;     // stage of completion the ornament is at

public:
    Ornament();
    virtual ~Ornament();

    virtual void Init();
    virtual void Update(double dt);     // check if ornament completion stage can be increased

    virtual void SetSprite();   // edit the mesh TexCoords

    void AddPart();
    bool isComplete();      // check if ornament has finished building

    void RemoveOrnament();
    
    // Getters
    int GetPartCount();
};

#endif