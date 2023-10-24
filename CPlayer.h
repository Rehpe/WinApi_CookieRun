#pragma once
#include "CObj.h"

//플레이어블 쿠키로, 모든 쿠키는 이 클래스를 상속받는다
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
    float       m_fHp;           //플레이어의 체력
    float       m_fMaxHp;        //플레이어의 최대체력
    float       m_fHpReduction;     //플레이어 체력 감소량
    float       m_fSpeed;        //플레이어의 이동 속력
    bool        m_bIsCrashed;    //장애물에 부딪쳤는가
    bool        m_bIsJump;       //점프 중인가
    int         m_iJumpCount;    //점프 횟수

    UINT        m_iScore;              //점수
    UINT        m_iCoin;                //코인

    Vec2        m_vSavePos;     //보너스 타임 진입 전 위치

    bool        m_bInvincible;      //현재 무적 상태인지
    float       m_fInvAccTime;      //무적 누적시간 기록
    bool        m_bBooster;         //부스터 상태인지
    float       m_fBoosterAccTime;  //부스터 누적시간 기록
    bool        m_bBigger;          //거대화 상태인지
    float       m_fBiggerAccTime;   //거대화 누적시간 기록
    bool        m_bBonus;           //보너스 상태인지
    float       m_fBonusAccTime;    //보너스 누적시간 기록
    float       m_fBonusMaxTime;       

    float   m_fSkillCoolTime;       //스킬 쿨타임
    float   m_fSkillAccTime;        //스킬 누적시간 
    float   m_fSkillMaxTime;        //스킬 총 사용시간
    float   m_fSkillUsingAccTime;   //스킬 사용누적시간

    bool    m_bMatchBonus;          //매치보너스

    bool    m_bOnSkill;             //스킬 사용 중인지

    bool    m_bOnShield;            //보호막 상태인지

    bool    m_bOnSummon;

    bool    m_bPlayerDead;          //게임오버인지      

    float   m_fVFSDelay;
   
    CTexture* m_pBoostVFXTex;

    CSound* m_pJumpSFX;             //점프 효과음
    CSound* m_pSlideSFX;            //슬라이드 효과음
    CSound* m_pBiggerLandSFX;       //거대화시 착지 효과음
    CSound* m_pBiggerJumpSFX;       //거대화시 점프 효과음
    CSound* m_pCrashSFX;            //부딪치는 효과음
    CSound* m_pInvincibleCrashSFX;  //무적상태에서 부딪치는 효과음
    CSound* m_pBiggerEndSFX;        //거대화 종료 효과음
    CSound* m_pGameOverSFX;         //죽음 효과음


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

