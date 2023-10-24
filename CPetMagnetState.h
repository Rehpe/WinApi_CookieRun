#pragma once
#include "CState.h"

class CSound;

class CPetMagnetState :
    public CState
{
public:
    CPetMagnetState();
    ~CPetMagnetState();

public:
    CLONE_DEACTIVATE(CPetMagnetState);

private:
    CSound*     m_pMagnetSFX;

public:
    virtual void final_tick() override;
    virtual void Enter() override;
    virtual void Exit() override;
};

