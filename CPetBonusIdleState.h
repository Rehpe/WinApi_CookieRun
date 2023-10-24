#pragma once
#include "CState.h"
class CPetBonusIdleState :
    public CState
{
public:
    CPetBonusIdleState();
    ~CPetBonusIdleState();

public:
    CLONE_DEACTIVATE(CPetBonusIdleState);

private:

public:
    virtual void final_tick() override;
    virtual void Enter() override;
    virtual void Exit() override;
};

