#pragma once
#include "CButtonUI.h"
class CSlideBtn :
    public CButtonUI
{
public:
    CSlideBtn();
    ~CSlideBtn();

public:
    CLONE_DEACTIVATE(CSlideBtn);

private:

public:
    virtual void tick() override;
    virtual void render(HDC _dc) override;

};

