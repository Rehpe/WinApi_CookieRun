#pragma once
#include "CPlayer.h"

class CMilkShield;
class CSound;
class CCookieSkillBar;

class CMilkCookie :
    public CPlayer
{

public:
    CMilkCookie();
    ~CMilkCookie();

public:
    CLONE(CMilkCookie);

private:
    CMilkShield* m_pShield;
    int          m_iShieldScore;
    CSound*      m_pSkillEnterSFX;
    CCookieSkillBar* m_pSkillBar;

public:
    void init();
    virtual void tick() override;
    virtual void render(HDC _dc) override;
    void Skill();


    virtual void BeginOverlap(CCollider* _pOther) override {};
    virtual void OnOverlap(CCollider* _pOther) override {};
    virtual void EndOverlap(CCollider* _pOther) override {};

public:
    void SetShield(CMilkShield* _shield) { m_pShield = _shield; }
    CMilkShield* GetShield() { return m_pShield; }

    int GetShieldScore() { return m_iShieldScore; };
    void SetShieldScore(int _i) { m_iShieldScore = _i; };

    CSound* GetSkillEnterSFX() { return m_pSkillEnterSFX; };
};

