#include "pch.h"
#include "CSnowSugarSkillEnterState.h"
#include "CSnowSugarCookie.h"
#include "CAnimator.h"
#include "CAnimation.h"
#include "CSound.h"

CSnowSugarSkillEnterState::CSnowSugarSkillEnterState()
{
}

CSnowSugarSkillEnterState::~CSnowSugarSkillEnterState()
{
}

void CSnowSugarSkillEnterState::final_tick()
{
	//어떤 쿠키인지 알아낸다.
	CSnowSugarCookie* pCookie = dynamic_cast<CSnowSugarCookie*>(GetOwnerFSM()->GetOwner());

	//상태 주인이 우유맛 쿠키가 아니라면 에러처리
	assert(pCookie);

	//============SnowSugarSkillEnter에서 실행할 것
	//pCookie->GetAnimator()->SetOffset(Vec2(0.f, 120.f));

	//=============SnowSugarSkillEnter의 전환 조건

	//SkillEnter 애니메이션이 종료되면
	if (pCookie->GetAnimator()->GetCurAnimation()->IsFinish())
	{
		GetOwnerFSM()->ChangeState(L"Skill");
		return;
	}
}

void CSnowSugarSkillEnterState::Enter()
{
	//어떤 쿠키인지 알아낸다.
	CSnowSugarCookie* pCookie = dynamic_cast<CSnowSugarCookie*>(GetOwnerFSM()->GetOwner());

	//SkillEnter 애니메이션을 재생한다
	pCookie->GetAnimator()->Play(L"SkillEnter", false);

	//SkillEnter 효과음을 재생한다.
	pCookie->GetSkillEnterSFX()->Play();
}

void CSnowSugarSkillEnterState::Exit()
{
	//어떤 쿠키인지 알아낸다.
	CSnowSugarCookie* pCookie = dynamic_cast<CSnowSugarCookie*>(GetOwnerFSM()->GetOwner());

	//해당 State에서 사용한 모든 애니메이션을 Reset시킨다
	pCookie->GetAnimator()->FindAnimation(L"SkillEnter")->Reset();

}
