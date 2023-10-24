#pragma once
#include "CLevel.h"

class CPanelUI;
class CButtonUI;
class CSound;

class CIntroLevel :
    public CLevel
{
public:
    CIntroLevel();
    ~CIntroLevel();

public:
    CLONE_DEACTIVATE(CIntroLevel);

private:
    CPanelUI*     m_pPanelUI;
    CButtonUI*    m_pPlayBtn;
    CButtonUI*    m_pEditBtn;
    CSound*       m_pBGM;

public:
    virtual void init() override;
    virtual void tick() override;
    virtual void Enter() override;
    virtual void Exit() override;
};

