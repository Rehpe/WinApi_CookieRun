#pragma once
#include "CProgressBarUI.h"
class CCookieSkillBar :
    public CProgressBarUI
{
public:
    CCookieSkillBar();
    ~CCookieSkillBar();

public:
    CLONE(CCookieSkillBar);

private:

public:
    virtual void tick() override;
    virtual void render(HDC _dc) override;

};

