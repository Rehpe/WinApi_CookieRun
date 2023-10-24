#pragma once
#include "CProgressBarUI.h"

class CTexture;
class CBackgroundEffect;

class CHpBarUI :
    public CProgressBarUI
{
public:
    CHpBarUI();
    ~CHpBarUI();

public:
    CLONE(CHpBarUI);

private:
    CTexture*   m_pEffect;
    CBackgroundEffect*    m_pBGEffect;

public:
    virtual void tick() override;
    virtual void render(HDC _dc) override;

public:
    void Effect_render(HDC _dc);
};

