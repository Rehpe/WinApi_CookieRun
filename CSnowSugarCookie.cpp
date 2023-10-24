#include "pch.h"
#include "CSnowSugarCookie.h"
#include "CResourceManager.h"
#include "CAnimator.h"
#include "CFSM.h"
#include "CRunState.h"
#include "CJumpState.h"
#include "CSlideState.h"
#include "CDoubleJumpState.h"
#include "CCrashState.h"
#include "CCrashDeadState.h"
#include "CBonusEnterState.h"
#include "CBonusIdleState.h"
#include "CBonusJumpState.h"
#include "CBonusExitState.h"
#include "CDeadState.h"
#include "CSnowSugarSkillEnterState.h"
#include "CSnowSugarSkillState.h"
#include "CSnowSugarSkillExitState.h"
#include "CKeyManager.h"
#include "CTimeManager.h"
#include <time.h>
#include "CJelly.h"
#include "CEffect.h"

CSnowSugarCookie::CSnowSugarCookie()
	: m_iSnowJellyScore(1000)
	, m_pVFX(nullptr)
{
	
	SetName(L"SnowSugarCookie");

	//아틀라스 텍스처 Load
	CTexture* pCookieTex = CResourceManager::GetInst()->LoadTexture(L"SnowSugar_Atlas", L"texture\\cookie\\SnowSugarCookie.png");

	//Animation Load
	GetAnimator()->LoadAnimation(L"animation\\Cookie\\SnowSugarCookie\\SnowSugarCookie_Run.anim");
	GetAnimator()->LoadAnimation(L"animation\\Cookie\\SnowSugarCookie\\SnowSugarCookie_Jump.anim");
	GetAnimator()->LoadAnimation(L"animation\\Cookie\\SnowSugarCookie\\SnowSugarCookie_Slide.anim");
	GetAnimator()->LoadAnimation(L"animation\\Cookie\\SnowSugarCookie\\SnowSugarCookie_DoubleJump.anim");
	GetAnimator()->LoadAnimation(L"animation\\Cookie\\SnowSugarCookie\\SnowSugarCookie_Crash.anim");
	GetAnimator()->LoadAnimation(L"animation\\Cookie\\SnowSugarCookie\\SnowSugarCookie_CrashDead.anim");
	GetAnimator()->LoadAnimation(L"animation\\Cookie\\SnowSugarCookie\\SnowSugarCookie_Dead.anim");
	GetAnimator()->LoadAnimation(L"animation\\Cookie\\SnowSugarCookie\\SnowSugarCookie_Boost.anim");
	GetAnimator()->LoadAnimation(L"animation\\Cookie\\SnowSugarCookie\\SnowSugarCookie_BonusEnter.anim");
	GetAnimator()->LoadAnimation(L"animation\\Cookie\\SnowSugarCookie\\SnowSugarCookie_BonusIdle.anim");
	GetAnimator()->LoadAnimation(L"animation\\Cookie\\SnowSugarCookie\\SnowSugarCookie_BonusJump.anim");
	GetAnimator()->LoadAnimation(L"animation\\Cookie\\SnowSugarCookie\\SnowSugarCookie_BonusExit.anim");

	//Skill Animation Load
	GetAnimator()->LoadAnimation(L"animation\\Cookie\\SnowSugarCookie\\SnowSugarCookie_SkillEnter.anim");
	GetAnimator()->LoadAnimation(L"animation\\Cookie\\SnowSugarCookie\\SnowSugarCookie_SkillIdle.anim");
	GetAnimator()->LoadAnimation(L"animation\\Cookie\\SnowSugarCookie\\SnowSugarCookie_SkillJump.anim");
	GetAnimator()->LoadAnimation(L"animation\\Cookie\\SnowSugarCookie\\SnowSugarCookie_SkillExit.anim");

	GetFSM()->AddState(L"Run", new CRunState);
	GetFSM()->AddState(L"Jump", new CJumpState);
	GetFSM()->AddState(L"Slide", new CSlideState);
	GetFSM()->AddState(L"DoubleJump", new CDoubleJumpState);
	GetFSM()->AddState(L"Dead", new CDeadState);
	GetFSM()->AddState(L"Crash", new CCrashState);
	GetFSM()->AddState(L"CrashDead", new CCrashDeadState);
	GetFSM()->AddState(L"BonusEnter", new CBonusEnterState);
	GetFSM()->AddState(L"BonusIdle", new CBonusIdleState);
	GetFSM()->AddState(L"BonusJump", new CBonusJumpState);
	GetFSM()->AddState(L"BonusExit", new CBonusExitState);
	GetFSM()->AddState(L"SkillEnter", new CSnowSugarSkillEnterState);
	GetFSM()->AddState(L"Skill", new CSnowSugarSkillState);
	GetFSM()->AddState(L"SkillExit", new CSnowSugarSkillExitState);

	//처음 상태는 무조건 Run
	GetFSM()->ChangeState(L"Run");

	SetScale(Vec2(60.f, 100.f));

	m_fSkillAccTime = 28.f;
	m_fSkillCoolTime = 30.f;
	m_fSkillMaxTime = 13.f;

	//점프, 슬라이드 효과음 부여
	m_pJumpSFX = CResourceManager::GetInst()->LoadSound(L"SnowSugar_Jump", L"sound\\Cookie\\SnowSugar_Jump.wav");
	m_pSlideSFX = CResourceManager::GetInst()->LoadSound(L"SnowSugar_Slide", L"sound\\Cookie\\SnowSugar_Slide.wav");
	m_pSkillEnterSFX = CResourceManager::GetInst()->LoadSound(L"SnowSugar_SkillEnter", L"sound\\Cookie\\SnowSugar_SkillEnter.wav");
	m_pSkillExitSFX = CResourceManager::GetInst()->LoadSound(L"SnowSugar_SkillExit", L"sound\\Cookie\\SnowSugar_SkillExit.wav");
}

CSnowSugarCookie::~CSnowSugarCookie()
{
}

void CSnowSugarCookie::init()
{
	//MatchBonus 상태라면
	if (m_bMatchBonus)
	{
		//m_iShieldScore += 5000;
	}
}

void CSnowSugarCookie::tick()
{
	Skill();
	CPlayer::tick();
}

void CSnowSugarCookie::render(HDC _dc)
{
	CPlayer::render(_dc);
}

void CSnowSugarCookie::Skill()
{
	if (m_bOnSkill == true && m_bOnSummon == false)
	{
		GetFSM()->ChangeState(L"SkillEnter");
		m_bOnSummon = true;
	}

	if (m_bOnSkill == true && m_bOnSummon == true)
	{
		if (m_pVFX == nullptr)
		{
			//이펙트 재생
			m_pVFX = new CEffect;
			//CTexture* m_pEffectTex = CResourceManager::GetInst()->LoadTexture(L"SnowSugarCookie_Summon_VFX", L"texture\\cookie\\SnowSugarCookie_Summon_VFX.png");
			//m_pVFX->SetEffectTex(m_pEffectTex);
			m_pVFX->CreateAnimator();
			m_pVFX->GetAnimator()->LoadAnimation(L"animation\\Cookie\\SnowSugarCookie\\SnowSugarCookie_Summon_VFX.anim");
			m_pVFX->SetRepeat(true);
			Vec2 vPos = Vec2(GetPos().x, 535.f);
			Instantiate(m_pVFX, vPos, LAYER::EFFECT);
			m_pVFX->GetAnimator()->Play(L"SnowSugar_Summon_VFX", true);
		}

		if (m_pVFX != nullptr)
			m_pVFX->SetPos(Vec2(GetPos().x, 535.f));

		m_fDelay += DT;

		if (m_fDelay >= 0.3f)
		{
			CJelly* pSnowJelly = new CJelly;
			pSnowJelly->init(ITEM_TYPE::SKILLJELLY_SNOWSUGAR);

			float posY = ((rand() % 390) + 200);	//200 ~ 590 중 랜덤

			Vec2 vPos = Vec2(GetPos().x + 650.f, posY);
			Instantiate(pSnowJelly, vPos, LAYER::JELLY);
			m_fDelay = 0;
		}
	}

	if(m_bOnSkill == false && m_bOnSummon == true)
	{
		GetFSM()->ChangeState(L"SkillExit");
		m_bOnSummon = false;
		m_pVFX->SetDead();
		m_pVFX = nullptr;
	}
}
