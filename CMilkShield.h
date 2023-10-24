#pragma once
#include "CObj.h"

class CTexture;
class CEffect;
class CSound;

class CMilkShield :
    public CObj
{
public:
    CMilkShield();
    ~CMilkShield();

public:
    CLONE(CMilkShield);

private:
    CTexture* m_pTex;
    CTexture* m_pEffectTex;
    CEffect*  m_pVFX;
    CSound*   m_pShieldSFX;

public:
    virtual void tick();
    virtual void render(HDC _dc);

private:
    virtual void BeginOverlap(CCollider* _pOther);
    virtual void OnOverlap(CCollider* _pOther);
    virtual void EndOverlap(CCollider* _pOther);
};

