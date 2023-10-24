#pragma once
#include "CProgressBarUI.h"
class CBonusBar :
    public CProgressBarUI
{
public:
    CBonusBar();
    ~CBonusBar();

public:
    CLONE(CBonusBar);

private:

public:
    virtual void tick() override;
    virtual void render(HDC _dc) override;

public:
};

