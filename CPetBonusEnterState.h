#pragma once
#include "CState.h"
class CPetBonusEnterState :
    public CState
{
public:
    CPetBonusEnterState();
    ~CPetBonusEnterState();

public:
    CLONE_DEACTIVATE(CPetBonusEnterState);

private:

public:
    virtual void final_tick() override;
    virtual void Enter() override;
    virtual void Exit() override;
};

