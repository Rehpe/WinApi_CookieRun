#pragma once
#include "CState.h"

class CJelly;

class CFlowerPodSkillState :
    public CState
{
public:
    CFlowerPodSkillState();
    ~CFlowerPodSkillState();

public:
    CLONE_DEACTIVATE(CFlowerPodSkillState);

private:
    vector<CJelly*>     m_vecFlowerJelly;
    float               m_fAccTime;
    float               m_fDelay;

public:
    virtual void final_tick() override;
    virtual void Enter() override;
    virtual void Exit() override;

};

