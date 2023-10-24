#pragma once
#include "CObj.h"

class CTexture;

class CEffect :
    public CObj
{
public:
    CEffect();
    ~CEffect();

public:
    CLONE(CEffect);

protected:
    CTexture* m_pTexture;
    float     m_fAccTime;
    float     m_fMaxTime;
    bool     m_bAlphaReduce;
    bool     m_bBlink;
    float    m_fSizeRatio;
    bool     m_Repeat;

public:
    virtual void render(HDC _dc) override;

public:
    void SetEffectTex(CTexture* _pTex) { m_pTexture = _pTex; };
    void SetAccTime(float _f) { m_fAccTime = _f; };
    void SetMaxTime(float _f) { m_fMaxTime = _f; };
    void SetAlphaReduce(bool _b) { m_bAlphaReduce = _b; };
    void SetBlink(bool _b) { m_bBlink = _b; };
    void SetSizeRatio(float _f) { m_fSizeRatio = _f; };
    void SetRepeat(bool _b) { m_Repeat = _b; };

};

