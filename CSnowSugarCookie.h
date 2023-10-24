#pragma once
#include "CPlayer.h"

class CSound;
class CEffect;

class CSnowSugarCookie :
    public CPlayer
{
public:
    CSnowSugarCookie();
    ~CSnowSugarCookie();

public:
    CLONE(CSnowSugarCookie);

private:
    CSound* m_pSkillEnterSFX;
    CSound* m_pSkillExitSFX;

    int     m_iSnowJellyScore;

    float   m_fDelay;
    CEffect* m_pVFX;

public:
    void init();
    virtual void tick() override;
    virtual void render(HDC _dc) override;
    void Skill();

public:
    virtual void BeginOverlap(CCollider* _pOther) override {};
    virtual void OnOverlap(CCollider* _pOther) override {};
    virtual void EndOverlap(CCollider* _pOther) override {};

public:
    CSound* GetSkillEnterSFX() { return m_pSkillEnterSFX; };
    CSound* GetSkillExitSFX() { return m_pSkillExitSFX; };

public:
    int GetSnowJellyScore() { return m_iSnowJellyScore; };
};

