#pragma once
#include "CObj.h"

class CPlayer;
class CTexture;

class CPet :
    public CObj
{
public:
    CPet();
    virtual ~CPet();

public:
    CLONE(CPet);

protected:
    CTexture*   m_pPetTex;
    CPlayer*    m_pPlayer;
    bool        m_bIsMagnet;
    float       m_fMagnetAccTime;
    bool        m_bOnSkill;     //스킬 사용중인지
    float       m_fSkillAccTime;
    float       m_fSkillCoolTime;


    Vec2        m_vDefaultOffset;

public:
    virtual void tick();
    virtual void render(HDC _dc);

public:
    void SetPetTex(CTexture* _pTex) { m_pPetTex = _pTex; };

    Vec2 GetDefaultOffset() { return m_vDefaultOffset; };

    void SetMagnet(bool _b) { m_bIsMagnet = _b; };
    bool IsMagnet() { return m_bIsMagnet; };

    CPlayer* GetPlayer() { return m_pPlayer; };
    void SetPlayer(CPlayer* _player) { m_pPlayer = _player; };

    bool IsOnSkill() { return m_bOnSkill; };
    void SetOnSkill(bool _b) { m_bOnSkill = _b; };

public:
    void MagnetCheck(float _fTime);
    void SkillCoolDownCheck();
};

