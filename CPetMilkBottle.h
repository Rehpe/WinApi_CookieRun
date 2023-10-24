#pragma once
#include "CPet.h"

class CSound;

class CPetMilkBottle :
    public CPet
{
public:
    CPetMilkBottle();
    ~CPetMilkBottle();

public:
    CLONE(CPetMilkBottle);

private:
    CSound* m_pMakeMilkJellySFX;

public:
    virtual void tick();
    virtual void render(HDC _dc);

public:
    CSound* GetMakeMilkJellySFX() { return m_pMakeMilkJellySFX; };

};

