#include "pch.h"
#include "CFairyCookie.h"
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
#include "CFairyShield.h"
#include "CLevelManager.h"
#include "CLevel.h"
#include "CSound.h"
#include "CPetFlowerPod.h"

CFairyCookie::CFairyCookie()
{
	SetName(L"FairyCookie");

	//아틀라스 텍스처 Load
	CTexture* pCookieTex = CResourceManager::GetInst()->LoadTexture(L"Fairy_Atlas", L"texture\\cookie\\FairyCookie.png");

	//Run
	GetAnimator()->LoadAnimation(L"animation\\Cookie\\FairyCookie\\FairyCookie_Run.anim");
	//Jump
	GetAnimator()->LoadAnimation(L"animation\\Cookie\\FairyCookie\\FairyCookie_Jump.anim");
	//Slide
	GetAnimator()->LoadAnimation(L"animation\\Cookie\\FairyCookie\\FairyCookie_Slide.anim");
	//DoubleJump
	GetAnimator()->LoadAnimation(L"animation\\Cookie\\FairyCookie\\FairyCookie_DoubleJump.anim");
	//Crash
	GetAnimator()->LoadAnimation(L"animation\\Cookie\\FairyCookie\\FairyCookie_Crash.anim");
	//Crash_Dead
	GetAnimator()->LoadAnimation(L"animation\\Cookie\\FairyCookie\\FairyCookie_CrashDead.anim");
	//Dead
	GetAnimator()->LoadAnimation(L"animation\\Cookie\\FairyCookie\\FairyCookie_Dead.anim");
	//Boost
	GetAnimator()->LoadAnimation(L"animation\\Cookie\\FairyCookie\\FairyCookie_Boost.anim");
	//BonusEnter
	GetAnimator()->LoadAnimation(L"animation\\Cookie\\FairyCookie\\FairyCookie_BonusEnter.anim");
	//BonusIdle
	GetAnimator()->LoadAnimation(L"animation\\Cookie\\FairyCookie\\FairyCookie_BonusIdle.anim");
	//BonusJump
	GetAnimator()->LoadAnimation(L"animation\\Cookie\\FairyCookie\\FairyCookie_BonusJump.anim");
	//BonusExit
	GetAnimator()->LoadAnimation(L"animation\\Cookie\\FairyCookie\\FairyCookie_BonusExit.anim");


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


	//처음 상태는 무조건 Run
	GetFSM()->ChangeState(L"Run");

	m_fSkillAccTime = 29.f;
	m_fSkillCoolTime = 30.f;

	//점프, 슬라이드 효과음 부여
	m_pJumpSFX = CResourceManager::GetInst()->LoadSound(L"Fairy_Jump", L"sound\\Cookie\\Fairy_Jump.wav");
	m_pSlideSFX = CResourceManager::GetInst()->LoadSound(L"Fairy_Slide", L"sound\\Cookie\\Fairy_Slide.wav");
	m_pShieldInitSFX = CResourceManager::GetInst()->LoadSound(L"Fairy_Shield", L"sound\\Cookie\\Fairy_Shield.wav");
}

CFairyCookie::~CFairyCookie()
{
}

void CFairyCookie::init()
{
	//MatchBonus 상태라면
	if (m_bMatchBonus)
	{
		CPetFlowerPod* pPet = dynamic_cast<CPetFlowerPod*>(CLevelManager::GetInst()->GetCurLevel()->GetPet());
		if (pPet == nullptr)
			return;

		pPet->SetFlowerJellyScore(20000);
	}
}

void CFairyCookie::tick()
{
	Skill();

	if (m_pShield != nullptr)
		m_pShield->SetPos(GetPos());
	
	CPlayer::tick();
}

void CFairyCookie::render(HDC _dc)
{
	CPlayer::render(_dc);
}

void CFairyCookie::Skill()
{
	if (m_bOnSkill)
	{
		if (m_pShield == nullptr)
		{
			//방패 생성
			m_pShield = new CFairyShield;
			m_bOnShield = true;
			m_pShieldInitSFX->Play();
			CLevelManager::GetInst()->GetCurLevel()->AddObject(m_pShield, LAYER::PLAYER_SKILL);
		}
		else
		{
			if (IsCrashed())
			{
				m_pShield->SetDead();
				m_pShield = nullptr;
				m_bOnSkill = false;
			}
		}
	}
}

