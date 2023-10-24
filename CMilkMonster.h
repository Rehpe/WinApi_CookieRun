#pragma once
#include "CObj.h"

class CTexture;
class CEffect;
class CSound;

enum class MonsterSize
{
    SMALL,
    BIG,
};

class CMilkMonster :
    public CObj
{
public:
    CMilkMonster(MonsterSize m_eSize);
    ~CMilkMonster();

public:
    CLONE(CMilkMonster);

private:
    CTexture*   m_pTex;
    int         m_iHp;
    MonsterSize m_eSize;
    float       m_fAccTime;
    bool        m_bGoBack;

    CSound*     m_pRushSFX;
    CSound*     m_pExtinctSFX;


public:
    virtual void tick() override;
    virtual void render(HDC _dc) override;

private:
    virtual void BeginOverlap(CCollider* _pOther);
    virtual void OnOverlap(CCollider* _pOther);
    virtual void EndOverlap(CCollider* _pOther);

public:
    CSound* GetRushSFX() { return m_pRushSFX; };
    CSound* GetExtinctSFX() { return m_pExtinctSFX; };
};

