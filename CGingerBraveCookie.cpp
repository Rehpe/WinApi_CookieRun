#include "pch.h"
#include "CGingerBraveCookie.h"
#include "CResourceManager.h"
#include "CLevelManager.h"
#include "CLevel.h"
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
#include "CDeadState.h"


CGingerBraveCookie::CGingerBraveCookie()
{
	SetName(L"GingerBraveCookie");

	//아틀라스 텍스처 Load
	CTexture* pCookieTex = CResourceManager::GetInst()->LoadTexture(L"GingerBrave_Atlas", L"texture\\cookie\\GingerBrave.png");

	//애니메이션 Load 
	GetAnimator()->LoadAnimation(L"animation\\Cookie\\GingerBrave\\GingerBrave_Run.anim");
	GetAnimator()->LoadAnimation(L"animation\\Cookie\\GingerBrave\\GingerBrave_Jump.anim");
	GetAnimator()->LoadAnimation(L"animation\\Cookie\\GingerBrave\\GingerBrave_Slide.anim");
	GetAnimator()->LoadAnimation(L"animation\\Cookie\\GingerBrave\\GingerBrave_DoubleJump.anim");
	GetAnimator()->LoadAnimation(L"animation\\Cookie\\GingerBrave\\GingerBrave_Crash.anim");
	GetAnimator()->LoadAnimation(L"animation\\Cookie\\GingerBrave\\GingerBrave_CrashDead.anim");
	GetAnimator()->LoadAnimation(L"animation\\Cookie\\GingerBrave\\GingerBrave_Dead.anim");
	GetAnimator()->LoadAnimation(L"animation\\Cookie\\GingerBrave\\GingerBrave_Boost.anim");
	GetAnimator()->LoadAnimation(L"animation\\Cookie\\GingerBrave\\GingerBrave_BonusEnter.anim");
	GetAnimator()->LoadAnimation(L"animation\\Cookie\\GingerBrave\\GingerBrave_BonusIdle.anim");
	GetAnimator()->LoadAnimation(L"animation\\Cookie\\GingerBrave\\GingerBrave_BonusJump.anim");
	GetAnimator()->LoadAnimation(L"animation\\Cookie\\GingerBrave\\GingerBrave_BonusExit.anim");

	GetFSM()->AddState(L"Run", new CRunState);
	GetFSM()->AddState(L"Jump", new CJumpState);
	GetFSM()->AddState(L"Slide", new CSlideState);
	GetFSM()->AddState(L"DoubleJump", new CDoubleJumpState);
	GetFSM()->AddState(L"Crash", new CCrashState);
	GetFSM()->AddState(L"Dead", new CDeadState);
	GetFSM()->AddState(L"CrashDead", new CCrashDeadState);
	GetFSM()->AddState(L"BonusEnter", new CBonusEnterState);
	GetFSM()->AddState(L"BonusIdle", new CBonusIdleState);
	GetFSM()->AddState(L"BonusJump", new CBonusJumpState);
	GetFSM()->AddState(L"BonusExit", new CBonusExitState);

	//처음 상태는 무조건 Run
	GetFSM()->ChangeState(L"Run");

	//점프, 슬라이드 효과음 부여
	m_pJumpSFX = CResourceManager::GetInst()->LoadSound(L"GingerBrave_Jump", L"sound\\Cookie\\GingerBrave_Jump.wav");
	m_pSlideSFX = CResourceManager::GetInst()->LoadSound(L"GingerBrave_Slide", L"sound\\Cookie\\GingerBrave_Slide.wav");


}

CGingerBraveCookie::~CGingerBraveCookie()
{
}

void CGingerBraveCookie::init()
{
	//MatchBonus 상태라면
	if (m_bMatchBonus)
	{
		m_fMaxHp += 10;
		m_fHp = m_fMaxHp;
	}
}

void CGingerBraveCookie::tick()
{
	CPlayer::tick();
}

void CGingerBraveCookie::render(HDC _dc)
{
	CPlayer::render(_dc);
}

void CGingerBraveCookie::BeginOverlap(CCollider* _pOther)
{
}

void CGingerBraveCookie::OnOverlap(CCollider* _pOther)
{
}

void CGingerBraveCookie::EndOverlap(CCollider* _pOther)
{
}
