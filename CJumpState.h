#pragma once
#include "CState.h"
class CJumpState :
    public CState
{
public:
    CJumpState();
    ~CJumpState();

public:
    CLONE(CJumpState);

private:

public:
    virtual void final_tick() override;
    virtual void Enter() override;
    virtual void Exit() override;
};

