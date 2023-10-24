#pragma once
#include "CLevel.h"

class CPanelUI;
class CTextUI;
class CButtonUI;
class CSound;

class CResultLevel :
    public CLevel
{
public:
    CResultLevel();
    ~CResultLevel();

public:
    CLONE_DEACTIVATE(CResultLevel);

private:
    CPanelUI*   m_pBackground;
    CButtonUI*  m_pOKBtn;

    CTextUI*    m_pScoreText;
    CTextUI*    m_pCoinText;

    int         m_iScore;
    int         m_iCoin;

    CSound*     m_pBGM;

public:
    virtual void init() override;
    virtual void tick() override;
    virtual void Enter() override;
    virtual void Exit() override;

public:
    void SetScore(int _i) { m_iScore = _i; };
    void SetCoin(int _i) { m_iCoin = _i; };

};

