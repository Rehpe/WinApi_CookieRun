#include "pch.h"
#include "CMilkCookie.h"
#include "CResourceManager.h"
#include "CAnimator.h"
#include "CAnimation.h"
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
#include "CTimeManager.h"
#include "CBackgroundEffect.h"
#include "CKeyManager.h"
#include "CMilkSkillState.h"
#include "CDeadState.h"
#include "CLevelManager.h"
#include "CLevel.h"

CMilkCookie::CMilkCookie()
	: m_iShieldScore(5000)
{
	SetName(L"MilkCookie");

	//아틀라스 텍스처 Load
	CTexture* pCookieTex = CResourceManager::GetInst()->LoadTexture(L"Milk_Atlas", L"texture\\cookie\\MilkCookie.png");

	//Run
	GetAnimator()->LoadAnimation(L"animation\\Cookie\\MilkCookie\\MilkCookie_Run.anim");
	//Jump
	GetAnimator()->LoadAnimation(L"animation\\Cookie\\MilkCookie\\MilkCookie_Jump.anim");
	//Slide
	GetAnimator()->LoadAnimation(L"animation\\Cookie\\MilkCookie\\MilkCookie_Slide.anim");
	//DoubleJump
	GetAnimator()->LoadAnimation(L"animation\\Cookie\\MilkCookie\\MilkCookie_DoubleJump.anim");
	//Crash
	GetAnimator()->LoadAnimation(L"animation\\Cookie\\MilkCookie\\MilkCookie_Crash.anim");
	//Crash_Dead
	GetAnimator()->LoadAnimation(L"animation\\Cookie\\MilkCookie\\MilkCookie_CrashDead.anim");
	//Dead
	GetAnimator()->LoadAnimation(L"animation\\Cookie\\MilkCookie\\MilkCookie_Dead.anim");
	//Boost
	GetAnimator()->LoadAnimation(L"animation\\Cookie\\MilkCookie\\MilkCookie_Boost.anim");
	//BonusEnter
	GetAnimator()->LoadAnimation(L"animation\\Cookie\\MilkCookie\\MilkCookie_BonusEnter.anim");
	//BonusIdle
	GetAnimator()->LoadAnimation(L"animation\\Cookie\\MilkCookie\\MilkCookie_BonusIdle.anim");
	//BonusJump
	GetAnimator()->LoadAnimation(L"animation\\Cookie\\MilkCookie\\MilkCookie_BonusJump.anim");
	//BonusExit
	GetAnimator()->LoadAnimation(L"animation\\Cookie\\MilkCookie\\MilkCookie_BonusExit.anim");

	//Skill Enter
	GetAnimator()->LoadAnimation(L"animation\\Cookie\\MilkCookie\\MilkCookie_SkillEnter.anim");

	//Skill Up
	GetAnimator()->LoadAnimation(L"animation\\Cookie\\MilkCookie\\MilkCookie_SkillUp.anim");

	//Skill Down
	GetAnimator()->LoadAnimation(L"animation\\Cookie\\MilkCookie\\MilkCookie_SkillDown.anim");
	
	//Skill Exit
	GetAnimator()->LoadAnimation(L"animation\\Cookie\\MilkCookie\\MilkCookie_SkillExit.anim");

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
	GetFSM()->AddState(L"Skill", new CMilkSkillState);

	//처음 상태는 무조건 Run
	GetFSM()->ChangeState(L"Run");

	m_fSkillAccTime = 28.f;
	m_fSkillCoolTime = 30.f;
	m_fSkillMaxTime = 13.f;

	//점프, 슬라이드 효과음 부여
	m_pJumpSFX = CResourceManager::GetInst()->LoadSound(L"Milk_Jump", L"sound\\Cookie\\Milk_Jump.wav");
	m_pSlideSFX = CResourceManager::GetInst()->LoadSound(L"Milk_Slide", L"sound\\Cookie\\Milk_Slide.wav");
	m_pSkillEnterSFX = CResourceManager::GetInst()->LoadSound(L"Milk_SkillEnter", L"sound\\Cookie\\Milk_SkillEnter.wav");

}

CMilkCookie::~CMilkCookie()
{
}

void CMilkCookie::init()
{
	//MatchBonus 상태라면
	if (m_bMatchBonus)
	{
		m_iShieldScore += 5000;
	}
}

void CMilkCookie::tick()
{
	Skill();
	CPlayer::tick();
}

void CMilkCookie::render(HDC _dc)
{
	CPlayer::render(_dc);
}

void CMilkCookie::Skill()
{
	if (GetFSM()->GetCurState()->GetKey() == L"Skill")
	{
		//무적
		//SetInvincible(true);

		//쿠키의 위치 제한
		if (GetPos().y <= -10)
		{
			SetPos(Vec2(GetPos().x, -10.f));
		}

		if (GetPos().y >= 700)
		{
			SetPos(Vec2(GetPos().x, 700.f));
		}

		//상승/하강 애니메이션 재생
		if(IsTap(KEY::SPACE))
		{
			GetAnimator()->Play(L"SkillUp", true);
		}

		if (IsRelease(KEY::SPACE))
		{
			if (GetAnimator()->GetCurAnimation()->IsFinish())
			{
				GetAnimator()->Play(L"SkillDown", true);
			}
		}
	}
}
