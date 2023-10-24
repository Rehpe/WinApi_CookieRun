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
	//� ��Ű���� �˾Ƴ���.
	CSnowSugarCookie* pCookie = dynamic_cast<CSnowSugarCookie*>(GetOwnerFSM()->GetOwner());

	//���� ������ ������ ��Ű�� �ƴ϶�� ����ó��
	assert(pCookie);

	//============SnowSugarSkillEnter���� ������ ��
	//pCookie->GetAnimator()->SetOffset(Vec2(0.f, 120.f));

	//=============SnowSugarSkillEnter�� ��ȯ ����

	//SkillEnter �ִϸ��̼��� ����Ǹ�
	if (pCookie->GetAnimator()->GetCurAnimation()->IsFinish())
	{
		GetOwnerFSM()->ChangeState(L"Skill");
		return;
	}
}

void CSnowSugarSkillEnterState::Enter()
{
	//� ��Ű���� �˾Ƴ���.
	CSnowSugarCookie* pCookie = dynamic_cast<CSnowSugarCookie*>(GetOwnerFSM()->GetOwner());

	//SkillEnter �ִϸ��̼��� ����Ѵ�
	pCookie->GetAnimator()->Play(L"SkillEnter", false);

	//SkillEnter ȿ������ ����Ѵ�.
	pCookie->GetSkillEnterSFX()->Play();
}

void CSnowSugarSkillEnterState::Exit()
{
	//� ��Ű���� �˾Ƴ���.
	CSnowSugarCookie* pCookie = dynamic_cast<CSnowSugarCookie*>(GetOwnerFSM()->GetOwner());

	//�ش� State���� ����� ��� �ִϸ��̼��� Reset��Ų��
	pCookie->GetAnimator()->FindAnimation(L"SkillEnter")->Reset();

}
