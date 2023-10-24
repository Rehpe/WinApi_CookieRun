#include "pch.h"
#include "CSnowSugarSkillExitState.h"
#include "CSnowSugarCookie.h"
#include "CAnimator.h"
#include "CAnimation.h"
#include "CSound.h"
#include "CCollider.h"

CSnowSugarSkillExitState::CSnowSugarSkillExitState()
{
}

CSnowSugarSkillExitState::~CSnowSugarSkillExitState()
{
}

void CSnowSugarSkillExitState::final_tick()
{
	//어떤 쿠키인지 알아낸다.
	CSnowSugarCookie* pCookie = dynamic_cast<CSnowSugarCookie*>(GetOwnerFSM()->GetOwner());

	//상태 주인이 우유맛 쿠키가 아니라면 에러처리
	assert(pCookie);

	//============SnowSugarSkillExitr에서 실행할 것
	pCookie->GetAnimator()->SetOffset(Vec2(0.f, -150.f));
	

	//=============SnowSugarSkillExit의 전환 조건

	//SkillEnter 애니메이션이 종료되면
	if (pCookie->GetAnimator()->GetCurAnimation()->IsFinish())
	{
		GetOwnerFSM()->ChangeState(L"Run");
		return;
	}
}

void CSnowSugarSkillExitState::Enter()
{
	//어떤 쿠키인지 알아낸다.
	CSnowSugarCookie* pCookie = dynamic_cast<CSnowSugarCookie*>(GetOwnerFSM()->GetOwner());

	//SkillEnter 애니메이션을 재생한다
	pCookie->GetAnimator()->Play(L"SkillExit", false);

	//SkillEnter 효과음을 재생한다.
	pCookie->GetSkillExitSFX()->Play();
}

void CSnowSugarSkillExitState::Exit()
{
	//어떤 쿠키인지 알아낸다.
	CSnowSugarCookie* pCookie = dynamic_cast<CSnowSugarCookie*>(GetOwnerFSM()->GetOwner());

	pCookie->SetInvincible(true);

	pCookie->SetOnSummon(false);

	pCookie->SetScale(Vec2(60.f, 100.f));
	pCookie->GetAnimator()->SetOffset(Vec2(0.f, 0.f));

	//해당 State에서 사용한 모든 애니메이션을 Reset시킨다
	pCookie->GetAnimator()->FindAnimation(L"SkillExit")->Reset();
}
