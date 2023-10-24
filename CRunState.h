#pragma once
#include "CState.h"
class CRunState :
    public CState
{

public:
    CRunState();
    ~CRunState();

public:
    CLONE(CRunState);

private:

public:
    virtual void final_tick() override;
    virtual void Enter() override;
    virtual void Exit() override;
};

