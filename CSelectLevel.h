#pragma once
#include "CLevel.h"

class CPanelUI;
class CButtonUI;
class CTexture;
class CSound;

class CSelectLevel :
    public CLevel
{
public:
    CSelectLevel();
    ~CSelectLevel();

public:
    CLONE_DEACTIVATE(CSelectLevel);

private:
    CSound* m_pBGM;

    CPanelUI* m_pPanelUI;
    CButtonUI* m_pPlayBtn;
    CButtonUI* m_pBackBtn;
    
    CPanelUI* m_pCookieAnim;
    CPanelUI* m_pPetAnim;

    vector<tCookieInfo>     m_vecCookieInfo;
    vector<tPetInfo>        m_vecPetInfo;

    CPanelUI* m_pCookieDesc;
    CPanelUI* m_pPetDesc;

    SELECT_COOKIE m_eCookieType;
    SELECT_PET m_ePetType;

    vector<CTexture*>   m_vecCookieDesc;
    vector<CTexture*>   m_vecPetDesc;

    bool    m_bMatchBonus;
    CPanelUI* m_pMatchBonus;

public:
    virtual void init() override;
    virtual void tick() override;
    virtual void Enter() override;
    virtual void Exit() override;

public:
    void SetCookieDesc(CPanelUI* _pDesc) { m_pCookieDesc = _pDesc; };
    CPanelUI* GetCookieDesc() { return m_pCookieDesc; };

    void SetPetDesc(CPanelUI* _pDesc) { m_pPetDesc = _pDesc; };
    CPanelUI* GetPetDesc() { return m_pPetDesc; };

    void SetCookieType(SELECT_COOKIE _eCookieType) { m_eCookieType = _eCookieType; };
    SELECT_COOKIE GetCookieType() { return m_eCookieType; };

    void SetPetType(SELECT_PET _ePetType) { m_ePetType = _ePetType; };
    SELECT_PET GetPetType() { return m_ePetType; };

    CPanelUI* GetCookieAnim() { return m_pCookieAnim; };
    CPanelUI* GetPetAnim() { return m_pPetAnim; };

    bool GetMatchBonus() { return m_bMatchBonus; };

private:
    void LoadCookieInfo();
    void LoadPetInfo();

    void CreateCookieBtn();
    void CreatePetBtn();

    void LoadCookieDesc();
    void LoadPetDesc();

public:
    void ChangeLoadLevel();
};

