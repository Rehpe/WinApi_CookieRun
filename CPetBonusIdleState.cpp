#include "pch.h"
#include "CPetBonusIdleState.h"
#include "CObj.h"
#include "CFSM.h"
#include "CAnimation.h"
#include "CAnimator.h"
#include "CPet.h"
#include "CLevelManager.h"
#include "CLevel.h"
#include "CPlayer.h"
#include "CKeyManager.h"

CPetBonusIdleState::CPetBonusIdleState()
{
}

CPetBonusIdleState::~CPetBonusIdleState()
{
}

void CPetBonusIdleState::final_tick()
{
	//============BonusIdleState���� ������ ��

	CPet* pPet = dynamic_cast<CPet*>(GetOwnerFSM()->GetOwner());
	assert(pPet);

	//��Ű�� ��ġ ��������
	//CPlayer* pCookie = CLevelManager::GetInst()->GetCurLevel()->GetPlayer();
	CPlayer* pCookie = pPet->GetPlayer();

	//���� ���ʽ� �������� ���� �� �÷��̾��� ���ʿ� �׻� �پ��ִ´�.
	Vec2 vPos = pCookie->GetPos() - Vec2(0.f, 100.f);

	if(pCookie->IsBigger())
		vPos = pCookie->GetPos() - Vec2(0.f, 180.f);

	pPet->SetPos(vPos);

	//=============BonusIdleState�� ��ȯ ����

	if (IsTap(KEY::SPACE))
	{
		GetOwnerFSM()->ChangeState(L"BonusJump");
		return;
	}
	if (!pCookie->IsBonus())
	{
		GetOwnerFSM()->ChangeState(L"BonusExit");
		return;
	}
}

void CPetBonusIdleState::Enter()
{
	//� ������ �˾Ƴ���.
	CPet* pPet = dynamic_cast<CPet*>(GetOwnerFSM()->GetOwner());

	//���� �ƴ϶�� ����ó��
	assert(pPet);

	//BonusIdle �ִϸ��̼��� ����Ѵ�
	GetOwnerFSM()->GetOwner()->GetAnimator()->Play(L"BonusIdle", true);
}

void CPetBonusIdleState::Exit()
{
	//�ش� State �ִϸ��̼��� Reset��Ų��
	GetOwnerFSM()->GetOwner()->GetAnimator()->FindAnimation(L"BonusIdle")->Reset();
}
