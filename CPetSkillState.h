#pragma once
#include "CState.h"
class CPetSkillState :
    public CState
{
public:
    CPetSkillState();
    ~CPetSkillState();

public:
    CLONE_DEACTIVATE(CPetSkillState);

private:

public:
    virtual void final_tick() override;
    virtual void Enter() override;
    virtual void Exit() override;
};

