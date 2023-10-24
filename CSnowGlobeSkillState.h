#pragma once
#include "CState.h"
class CSnowGlobeSkillState :
    public CState
{
public:
    CSnowGlobeSkillState();
    ~CSnowGlobeSkillState();

public:
    CLONE_DEACTIVATE(CSnowGlobeSkillState);

private:
    float               m_fDelay;

public:
    virtual void final_tick() override;
    virtual void Enter() override;
    virtual void Exit() override;

};

