#pragma once
#include "CButtonUI.h"

class CPlayer;
class CPet;

class CSelectBtn :
    public CButtonUI
{
public:
    CSelectBtn();
    ~CSelectBtn();

public:
    CLONE(CSelectBtn);

private:
    SELECT_COOKIE   m_eCookieType;
    SELECT_PET      m_ePetType;

    CTexture* m_pPreviewTex;
    CTexture* m_pDescTex;

public:
    void render(HDC _dc);

public:
    void SetDescTex(CTexture* _pTex) { m_pDescTex = _pTex; };
    CTexture* GetDescTex() { return m_pDescTex; };

    void SetCookieType(SELECT_COOKIE _eCookieType) { m_eCookieType = _eCookieType; };
    SELECT_COOKIE GetCookieType() { return m_eCookieType; };

    void SetPetType(SELECT_PET _ePetType) { m_ePetType = _ePetType; };
    SELECT_PET GetPetType() { return m_ePetType; };

public:
    void selectObject();
};

