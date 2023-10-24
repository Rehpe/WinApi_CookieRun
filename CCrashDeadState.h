#pragma once
#include "CState.h"
class CCrashDeadState :
    public CState
{
public:
    CCrashDeadState();
    ~CCrashDeadState();

public:
    CLONE(CCrashDeadState);

private:

public:
    virtual void final_tick() override;
    virtual void Enter() override;
    virtual void Exit() override;
};

