#pragma once
#include "CState.h"
class CCrashState :
    public CState
{
public:
    CCrashState();
    ~CCrashState();

public:
    CLONE(CCrashState);

private:

public:
    virtual void final_tick() override;
    virtual void Enter() override;
    virtual void Exit() override;
};

