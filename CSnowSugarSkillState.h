#pragma once
#include "CState.h"
class CSnowSugarSkillState :
    public CState
{
public:
    CSnowSugarSkillState();
    ~CSnowSugarSkillState();

public:
    CLONE(CSnowSugarSkillState);

private:
    float               m_fDelay;

public:
    virtual void final_tick() override;
    virtual void Enter() override;
    virtual void Exit() override;

};

