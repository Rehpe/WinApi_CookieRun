#pragma once
#include "CState.h"
class CSlideState :
    public CState
{
public:
    CSlideState();
    ~CSlideState();

public:
    CLONE(CSlideState);

private:

public:
    virtual void final_tick() override;
    virtual void Enter() override;
    virtual void Exit() override;
};

