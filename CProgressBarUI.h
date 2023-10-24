#pragma once
#include "CUI.h"

class CTexture;

class CProgressBarUI :
    public CUI
{
public:
    CProgressBarUI();
    ~CProgressBarUI();

public:
    CLONE(CProgressBarUI);

protected:
    CTexture*   m_pBGTex;
    CTexture*   m_pBarTex;
    CTexture*   m_pTailTex;

    Vec2        m_vBarPos;
    Vec2        m_vBarTail;         //줄어드는 바의 끝부분

    float       m_fMaxValue;
    float       m_fVarValue;
    float       m_fRatio;

public:
    virtual void tick();
    virtual void render(HDC _dc);

public:
    virtual void BG_render(HDC _dc);
    virtual void Bar_render(HDC _dc);
    virtual void Tail_render(HDC _dc);

public:
    void SetBGTex(CTexture* _pTex) { m_pBGTex = _pTex; };
    void SetBarTex(CTexture* _pTex) { m_pBarTex = _pTex; };
    void SetTailTex(CTexture* _pTex) { m_pTailTex = _pTex; };

    CTexture* GetBGTex() { return m_pBGTex; };
    CTexture* GetBarTex() { return m_pBarTex; };
    CTexture* GetTailTex() { return m_pTailTex; };

    void SetMaxValue(float _f) { m_fMaxValue = _f; };
    float GetMaxValue() { return m_fMaxValue; };
    
    void SetVarValue(float _f) { m_fVarValue = _f; };
    float GetVarValue() { return m_fVarValue; };

    void SetRatio(float _f) { m_fRatio = _f; };
    float GetRatio() { return m_fRatio; };


};

