#pragma once
#include "CState.h"
class CPetBonusJumpState :
    public CState
{
public:
    CPetBonusJumpState();
    ~CPetBonusJumpState();

public:
    CLONE_DEACTIVATE(CPetBonusJumpState);

private:

public:
    virtual void final_tick() override;
    virtual void Enter() override;
    virtual void Exit() override;
};

