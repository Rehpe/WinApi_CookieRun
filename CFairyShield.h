#pragma once
#include "CObj.h"

class CTexture;
class CEffect;
class CSound;

class CFairyShield :
    public CObj
{
public:
    CFairyShield();
    ~CFairyShield();

public:
    CLONE(CFairyShield);

private:
    CTexture* m_pShieldTex;
    CEffect* m_pVFX;
    CSound* m_pShieldSFX;

public:
    virtual void tick();
    virtual void render(HDC _dc);

private:
    virtual void BeginOverlap(CCollider* _pOther) {};
    virtual void OnOverlap(CCollider* _pOther) {};
    virtual void EndOverlap(CCollider* _pOther) {};
};

