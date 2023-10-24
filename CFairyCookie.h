#pragma once
#include "CPlayer.h"

class CCookieSkillBar;
class CFairyShield;

class CFairyCookie :
    public CPlayer
{
public:
    CFairyCookie();
    ~CFairyCookie();

public:
    CLONE(CFairyCookie);

private:
    CSound*             m_pShieldInitSFX;
    CCookieSkillBar*    m_pSkillBar;
    CFairyShield*       m_pShield;

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
    //CSound* GetSkillEnterSFX() { return m_pSkillEnterSFX; };
};

