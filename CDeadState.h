#pragma once
#include "CState.h"
class CDeadState :
    public CState
{
public:
    CDeadState();
    ~CDeadState();

public:
    CLONE(CDeadState);

private:

public:
    virtual void final_tick() override;
    virtual void Enter() override;
    virtual void Exit() override;

};

