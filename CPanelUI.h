#pragma once
#include "CUI.h"
class CPanelUI :
    public CUI
{
public:
    CPanelUI();
    ~CPanelUI();

public:
    CLONE(CPanelUI);

private:
    Vec2    m_vLeftRightPadding;
    Vec2    m_vTopBottomPadding;
    Vec2    m_vInnerPadding;


    Vec2    m_vPressPos;

public:
    float   m_fRatio;

public:
    virtual void tick() override;
    virtual void render(HDC _dc) override;

    virtual void MouseLbtnDown() override;
    //virtual void MouseLbtnClicked() override;
};

