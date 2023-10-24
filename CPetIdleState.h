#pragma once
#include "CState.h"
class CPetIdleState :
    public CState
{
public:
    CPetIdleState();
    ~CPetIdleState();

public:
    CLONE_DEACTIVATE(CPetIdleState);

private:

public:
    virtual void final_tick() override;
    virtual void Enter() override;
    virtual void Exit() override;
};

