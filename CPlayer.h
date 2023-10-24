#pragma once
#include "CObj.h"

//�÷��̾�� ��Ű��, ��� ��Ű�� �� Ŭ������ ��ӹ޴´�
class CTexture;
class CSound;

class CPlayer :
    public CObj
{
public:
    CPlayer();
    CPlayer(const CPlayer& _other);
    virtual ~CPlayer();

public:
    CLONE(CPlayer);

protected:
    float       m_fHp;           //�÷��̾��� ü��
    float       m_fMaxHp;        //�÷��̾��� �ִ�ü��
    float       m_fHpReduction;     //�÷��̾� ü�� ���ҷ�
    float       m_fSpeed;        //�÷��̾��� �̵� �ӷ�
    bool        m_bIsCrashed;    //��ֹ��� �ε��ƴ°�
    bool        m_bIsJump;       //���� ���ΰ�
    int         m_iJumpCount;    //���� Ƚ��

    UINT        m_iScore;              //����
    UINT        m_iCoin;                //����

    Vec2        m_vSavePos;     //���ʽ� Ÿ�� ���� �� ��ġ

    bool        m_bInvincible;      //���� ���� ��������
    float       m_fInvAccTime;      //���� �����ð� ���
    bool        m_bBooster;         //�ν��� ��������
    float       m_fBoosterAccTime;  //�ν��� �����ð� ���
    bool        m_bBigger;          //�Ŵ�ȭ ��������
    float       m_fBiggerAccTime;   //�Ŵ�ȭ �����ð� ���
    bool        m_bBonus;           //���ʽ� ��������
    float       m_fBonusAccTime;    //���ʽ� �����ð� ���
    float       m_fBonusMaxTime;       

    float   m_fSkillCoolTime;       //��ų ��Ÿ��
    float   m_fSkillAccTime;        //��ų �����ð� 
    float   m_fSkillMaxTime;        //��ų �� ���ð�
    float   m_fSkillUsingAccTime;   //��ų ��봩���ð�

    bool    m_bMatchBonus;          //��ġ���ʽ�

    bool    m_bOnSkill;             //��ų ��� ������

    bool    m_bOnShield;            //��ȣ�� ��������

    bool    m_bOnSummon;

    bool    m_bPlayerDead;          //���ӿ�������      

    float   m_fVFSDelay;
   
    CTexture* m_pBoostVFXTex;

    CSound* m_pJumpSFX;             //���� ȿ����
    CSound* m_pSlideSFX;            //�����̵� ȿ����
    CSound* m_pBiggerLandSFX;       //�Ŵ�ȭ�� ���� ȿ����
    CSound* m_pBiggerJumpSFX;       //�Ŵ�ȭ�� ���� ȿ����
    CSound* m_pCrashSFX;            //�ε�ġ�� ȿ����
    CSound* m_pInvincibleCrashSFX;  //�������¿��� �ε�ġ�� ȿ����
    CSound* m_pBiggerEndSFX;        //�Ŵ�ȭ ���� ȿ����
    CSound* m_pGameOverSFX;         //���� ȿ����


public:
    virtual void tick() override;
    virtual void render(HDC _dc) override;

    virtual void BeginOverlap(CCollider* _pOther) override;
    virtual void OnOverlap(CCollider* _pOther) override;
    virtual void EndOverlap(CCollider* _pOther) override;

    void InvincibleCheck(float _fTime);
    void BoosterCheck(float _fTime);
    void BiggerCheck(float _fTime);
    void BonusCheck(float _fTime);
    void SkillCoolDownCheck();
    void SkillUsingCheck();

    void LoadSFX();


public:
    void SetHP(float _hp) { m_fHp = _hp; };
    float GetHP() { return m_fHp; };

    void SetMaxHP(float _hp) { m_fMaxHp = _hp; };
    float GetMaxHP() { return m_fMaxHp; };

    void SetHpReduction(float _hp) { m_fHpReduction = _hp; };

    void SetSpeed(float _speed) { m_fSpeed = _speed; };
    float GetSpeed() { return m_fSpeed; }

    void SetCrashed(bool _b) { m_bIsCrashed = _b; };
    bool IsCrashed() { return m_bIsCrashed; };

    void SetJump(bool _b) { m_bIsJump = _b; };
    bool IsJump() { return m_bIsJump; };

    void SetJumpCount(int _count) { m_iJumpCount = _count; };
    int GetJumpCount() { return m_iJumpCount; };

    void SetInvincible(bool _b) { m_bInvincible = _b; };
    bool IsInvincible() { return m_bInvincible; };

    void SetBooster(bool _b) { m_bBooster = _b; };
    bool IsBooster() { return m_bBooster; };

    void SetBigger(bool _b) { m_bBigger = _b; };
    bool IsBigger() { return m_bBigger; };

    void SetBonus(bool _b) { m_bBonus = _b; };
    bool IsBonus() { return m_bBonus; };

    void SavePos() { m_vSavePos = GetPos() + Vec2(200.f, 0.f); }
    Vec2 LoadPos() { return m_vSavePos; }

    void AddScore(UINT _iScore) { m_iScore += _iScore; };
    UINT GetScore() { return m_iScore; };

    void AddCoin(UINT _iCoin) { m_iCoin += _iCoin; };
    UINT GetCoin() { return m_iCoin; };

    bool IsOnSkill() { return m_bOnSkill; };

    float GetBonusMaxTime() { return m_fBonusMaxTime; };
    float GetBonusAccTime() { return m_fBonusAccTime; };

    float GetSkillUsingAccTime() { return m_fSkillUsingAccTime; };
    float GetSkillMaxTime() { return m_fSkillMaxTime; };

    float GetSkillAccTime() { return m_fSkillAccTime; };
    float GetSkillCoolTime() { return m_fSkillCoolTime; };

    void SetPlayerDead(bool _b) { m_bPlayerDead = _b; };
    bool GetPlayerDead() { return m_bPlayerDead; };

    void SetMatchBonus(bool _b) { m_bMatchBonus = _b; };

    bool IsOnShield() { return m_bOnShield; };
    void SetOnShield(bool _b) { m_bOnShield = _b; };

    void SetOnSummon(bool _b) { m_bOnSummon = _b; };
    bool IsOnSummon() { return m_bOnSummon; };

	CSound* GetJumpSFX() { return m_pJumpSFX; };
	CSound* GetSlideSFX() { return m_pSlideSFX; };
	CSound* GetBiggerLandSFX() { return m_pBiggerLandSFX; };
	CSound* GetBiggerJumpSFX() { return m_pBiggerJumpSFX; };
	CSound* GetCrashSFX() { return m_pCrashSFX; };
	CSound* GetInvincibleCrashSFX() { return m_pInvincibleCrashSFX; };
	CSound* GetBiggerEndSFX() { return m_pBiggerEndSFX; };
	CSound* GetGameOverSFX() { return m_pGameOverSFX; };
   
};

