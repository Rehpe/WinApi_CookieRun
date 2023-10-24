#pragma once
#include "CButtonUI.h"
class CJumpBtn :
    public CButtonUI
{
public:
    CJumpBtn();
    ~CJumpBtn();

public:
    CLONE_DEACTIVATE(CJumpBtn);

private:

public:
    virtual void tick() override;
    virtual void render(HDC _dc) override;

};

