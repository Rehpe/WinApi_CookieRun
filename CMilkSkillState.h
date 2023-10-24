#pragma once
#include "CState.h"

class CBackgroundEffect;
class CMilkShield;

class CMilkSkillState :
    public CState
{
public:
    CMilkSkillState();
    ~CMilkSkillState();

public:
    CLONE(CMilkSkillState);

private:
    CBackgroundEffect*  m_pEffect;
    CMilkShield*        m_pMilkShield;
    vector<float>       m_vecDuration;
    float               m_fAccTime;

public:
    virtual void final_tick() override;
    virtual void Enter() override;
    virtual void Exit() override;
};

