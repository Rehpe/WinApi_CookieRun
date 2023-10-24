#pragma once
#include "CLevel.h"

class CTextUI;
class CSound;
class CButtonUI;
class CCookieSkillBar;

class CStage1 :
    public CLevel
{

public:
    CStage1();
    ~CStage1();

public:
    CLONE_DEACTIVATE(CStage1);

private:
    SELECT_COOKIE       m_eCookieType;
    SELECT_PET          m_ePetType;
    bool                m_bMatchBonus;

    CTextUI*            m_pScoreText;
    CTextUI*            m_pCoinText;
    CCookieSkillBar*    m_pSkillBar;

    CButtonUI*          m_pPauseBtn;

    CSound*             m_pBGM;

public:
    void SetCookieType(SELECT_COOKIE _eCookieType) { m_eCookieType = _eCookieType; };
    SELECT_COOKIE   GetCookieType() { return m_eCookieType; };

    void SetPetType(SELECT_PET _ePetType) { m_ePetType = _ePetType; };
    SELECT_PET  GetPetType() { return m_ePetType; };

    void SetMatchBonus(bool _b) { m_bMatchBonus = _b; };
    bool GetMatchBonus() { return m_bMatchBonus; };

    void SetScoreText(CTextUI* _UI) { m_pScoreText = _UI; };
    void SetCoinText(CTextUI* _UI) { m_pCoinText = _UI; };

	CTextUI* GetScoreText() { return m_pScoreText; }
	CTextUI* GetCoinText() { return m_pCoinText; }

public:
    virtual void init() override;
    virtual void tick() override;
    virtual void Enter() override;
    virtual void Exit() override;

private:
    void initUI();
    void CreateTextUI();

};

