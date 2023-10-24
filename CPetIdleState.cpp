#include "pch.h"
#include "CPetIdleState.h"
#include "CObj.h"
#include "CFSM.h"
#include "CAnimation.h"
#include "CAnimator.h"
#include "CPet.h"
#include "CLevelManager.h"
#include "CLevel.h"
#include "CPlayer.h"

CPetIdleState::CPetIdleState()
{
}

CPetIdleState::~CPetIdleState()
{
}

void CPetIdleState::final_tick()
{
	//============IdleState���� ������ ��
	
	CPet* pPet = dynamic_cast<CPet*>(GetOwnerFSM()->GetOwner());
	assert(pPet);

	//��Ű�� ��ġ ��������
	CPlayer* pCookie = CLevelManager::GetInst()->GetCurLevel()->GetPlayer();
	
	//���� �÷��̾��� ���ʿ� �׻� �پ��ִ´�.
	Vec2 vPos = pCookie->GetPos() - pPet->GetDefaultOffset();
	pPet->SetPos(vPos);

	//=============IdleState�� ��ȯ ����

	//�ڼ� ������ ȹ�� ��
	if (pPet->IsMagnet())
	{
		GetOwnerFSM()->ChangeState(L"Magnet");
		return;
	}

	//���ʽ� Ÿ�� ���� ��
	if (pCookie->IsBonus())
	{
		if (pCookie->IsOnSummon())
			return;

		GetOwnerFSM()->ChangeState(L"BonusEnter");
		return;
	}

	//��ų ����
	if (pPet->IsOnSkill())
	{
		GetOwnerFSM()->ChangeState(L"Skill");
		return;
	}

}


void CPetIdleState::Enter()
{
	//� ������ �˾Ƴ���.
	CPet* pPet = dynamic_cast<CPet*>(GetOwnerFSM()->GetOwner());

	//���� �ƴ϶�� ����ó��
	assert(pPet);

	//Idle �ִϸ��̼��� ����Ѵ�
	GetOwnerFSM()->GetOwner()->GetAnimator()->Play(L"Idle", true);
}

void CPetIdleState::Exit()
{
	//�ش� State �ִϸ��̼��� Reset��Ų��
	GetOwnerFSM()->GetOwner()->GetAnimator()->FindAnimation(L"Idle")->Reset();
}
