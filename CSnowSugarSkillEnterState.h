#pragma once
#include "CState.h"
class CSnowSugarSkillEnterState :
    public CState
{
public:
    CSnowSugarSkillEnterState();
    ~CSnowSugarSkillEnterState();

public:
    CLONE(CSnowSugarSkillEnterState);

private:
    float               m_fAccTime;

public:
    virtual void final_tick() override;
    virtual void Enter() override;
    virtual void Exit() override;
};

