#pragma once
#include "CPet.h"

class CSound;

class CPetSnowGlobe :
    public CPet
{
public:
    CPetSnowGlobe();
    ~CPetSnowGlobe();

public:
    CLONE(CPetSnowGlobe);

private:
    CSound* m_pMakeJellySFX;

public:
    virtual void tick();
    virtual void render(HDC _dc);

public:
    CSound* GetMakeJellySFX() { return m_pMakeJellySFX; };
};

