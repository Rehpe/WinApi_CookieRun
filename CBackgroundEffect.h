#pragma once
#include "CEffect.h"

enum class BACKGROUND_EFFECT
{
    NONE,
    SEMITRANSPARENT,
    LOWHP,

    END,
};

class CBackgroundEffect :
    public CEffect
{
public:
    CBackgroundEffect();
    ~CBackgroundEffect();

public:
    CLONE(CBackgroundEffect);

private:
    float m_fRatio;

public:
    void init(BACKGROUND_EFFECT _effect);
    virtual void render(HDC _dc);

};

