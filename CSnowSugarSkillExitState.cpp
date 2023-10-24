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
	//� ��Ű���� �˾Ƴ���.
	CSnowSugarCookie* pCookie = dynamic_cast<CSnowSugarCookie*>(GetOwnerFSM()->GetOwner());

	//���� ������ ������ ��Ű�� �ƴ϶�� ����ó��
	assert(pCookie);

	//============SnowSugarSkillExitr���� ������ ��
	pCookie->GetAnimator()->SetOffset(Vec2(0.f, -150.f));
	

	//=============SnowSugarSkillExit�� ��ȯ ����

	//SkillEnter �ִϸ��̼��� ����Ǹ�
	if (pCookie->GetAnimator()->GetCurAnimation()->IsFinish())
	{
		GetOwnerFSM()->ChangeState(L"Run");
		return;
	}
}

void CSnowSugarSkillExitState::Enter()
{
	//� ��Ű���� �˾Ƴ���.
	CSnowSugarCookie* pCookie = dynamic_cast<CSnowSugarCookie*>(GetOwnerFSM()->GetOwner());

	//SkillEnter �ִϸ��̼��� ����Ѵ�
	pCookie->GetAnimator()->Play(L"SkillExit", false);

	//SkillEnter ȿ������ ����Ѵ�.
	pCookie->GetSkillExitSFX()->Play();
}

void CSnowSugarSkillExitState::Exit()
{
	//� ��Ű���� �˾Ƴ���.
	CSnowSugarCookie* pCookie = dynamic_cast<CSnowSugarCookie*>(GetOwnerFSM()->GetOwner());

	pCookie->SetInvincible(true);

	pCookie->SetOnSummon(false);

	pCookie->SetScale(Vec2(60.f, 100.f));
	pCookie->GetAnimator()->SetOffset(Vec2(0.f, 0.f));

	//�ش� State���� ����� ��� �ִϸ��̼��� Reset��Ų��
	pCookie->GetAnimator()->FindAnimation(L"SkillExit")->Reset();
}
