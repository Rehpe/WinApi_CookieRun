#pragma once
#include "CLevel.h"

class CPanelUI;
class CTexture;
class CSound;

class CLoadLevel :
    public CLevel
{
public:
    CLoadLevel();
    ~CLoadLevel();

public:
    CLONE_DEACTIVATE(CLoadLevel);

private:
    SELECT_COOKIE       m_eCookieType;
    SELECT_PET          m_ePetType;
    bool                m_bMatchBonus;
    CPanelUI*           m_pBackground;
    CPanelUI*           m_pCookieImg;
    vector<CTexture*>   m_vecCookieTex;
    CSound*             m_pBGM;

public:
    virtual void init() override;
    virtual void tick() override;
    virtual void Enter() override;
    virtual void Exit() override;

public:
    void SetCookieType(SELECT_COOKIE _eCookieType) { m_eCookieType = _eCookieType; };
    SELECT_COOKIE   GetCookieType() { return m_eCookieType; };

    void SetPetType(SELECT_PET _ePetType) { m_ePetType = _ePetType; };
    SELECT_PET  GetPetType() { return m_ePetType; };

    void SetMatchBonus(bool _b) { m_bMatchBonus = _b; };
    bool GetMatchBonus() { return m_bMatchBonus; };

private:
    void LoadCookieTex();

};

