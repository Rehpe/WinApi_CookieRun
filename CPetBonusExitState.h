#pragma once
#include "CState.h"
class CPetBonusExitState :
    public CState
{
public:
    CPetBonusExitState();
    ~CPetBonusExitState();

public:
    CLONE_DEACTIVATE(CPetBonusExitState);

private:

public:
    virtual void final_tick() override;
    virtual void Enter() override;
    virtual void Exit() override;
};

