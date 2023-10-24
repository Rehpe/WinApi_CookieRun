#pragma once
#include "CState.h"
class CDoubleJumpState :
    public CState
{
public:
    CDoubleJumpState();
    ~CDoubleJumpState();

public:
    CLONE(CDoubleJumpState);

private:

public:
    virtual void final_tick() override;
    virtual void Enter() override;
    virtual void Exit() override;
};

