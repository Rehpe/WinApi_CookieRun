#pragma once
#include "CState.h"

class CJelly;

class CMilkBottleSkillState :
    public CState
{
public:
    CMilkBottleSkillState();
    ~CMilkBottleSkillState();

public:
    CLONE_DEACTIVATE(CMilkBottleSkillState);

private:
    vector<CJelly*>     m_vecMilkJelly;
    float               m_fAccTime;
    float               m_fDelay;

public:
    virtual void final_tick() override;
    virtual void Enter() override;
    virtual void Exit() override;
};

