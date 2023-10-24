#pragma once
#include "CPet.h"

class CSound;

class CPetFlowerPod :
    public CPet
{
public:
    CPetFlowerPod();
    ~CPetFlowerPod();

public:
    CLONE(CPetFlowerPod);

private:
    CSound* m_pMakeFlowerJellySFX;
    int     m_iFlowerJellyScore;

public:
    virtual void tick();
    virtual void render(HDC _dc);

public:
   CSound* GetMakeFlowerJellySFX() { return m_pMakeFlowerJellySFX; };

public:
    int GetFlowerJellyScore() { return m_iFlowerJellyScore; };
    void SetFlowerJellyScore(int _iScore) { m_iFlowerJellyScore = _iScore; };
};

