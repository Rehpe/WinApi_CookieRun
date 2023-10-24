#pragma once
#include "CState.h"
class CSnowSugarSkillExitState :
    public CState
{
public:
    CSnowSugarSkillExitState();
    ~CSnowSugarSkillExitState();

public:
    CLONE(CSnowSugarSkillExitState);

private:
    float               m_fAccTime;

public:
    virtual void final_tick() override;
    virtual void Enter() override;
    virtual void Exit() override;
};

