#include "pch.h"
#include "CPetBonusJumpState.h"
#include "CObj.h"
#include "CFSM.h"
#include "CAnimation.h"
#include "CAnimator.h"
#include "CPet.h"
#include "CLevelManager.h"
#include "CLevel.h"
#include "CPlayer.h"
#include "CKeyManager.h"

CPetBonusJumpState::CPetBonusJumpState()
{
}

CPetBonusJumpState::~CPetBonusJumpState()
{
}

void CPetBonusJumpState::final_tick()
{
	//============BonusJumpState���� ������ ��

	CPet* pPet = dynamic_cast<CPet*>(GetOwnerFSM()->GetOwner());
	assert(pPet);

	//��Ű�� ��ġ ��������
	CPlayer* pCookie = pPet->GetPlayer();

	//���� ���ʽ� �������� ���� �� �÷��̾��� ���ʿ� �׻� �پ��ִ´�.
	Vec2 vPos = pCookie->GetPos() - Vec2(0.f, 100.f);
	if (pCookie->IsBigger())
		vPos = pCookie->GetPos() - Vec2(0.f, 180.f);
	pPet->SetPos(vPos);

	//=============BonusJumpState�� ��ȯ ����

	if (IsRelease(KEY::SPACE))
	{
		GetOwnerFSM()->ChangeState(L"BonusIdle");
		return;
	}

	if (!pCookie->IsBonus())
	{
		GetOwnerFSM()->ChangeState(L"BonusExit");
		return;
	}
}

void CPetBonusJumpState::Enter()
{
	//� ������ �˾Ƴ���.
	CPet* pPet = dynamic_cast<CPet*>(GetOwnerFSM()->GetOwner());

	//���� �ƴ϶�� ����ó��
	assert(pPet);

	//BonusJump �ִϸ��̼��� ����Ѵ�
	GetOwnerFSM()->GetOwner()->GetAnimator()->Play(L"BonusJump", true);
}

void CPetBonusJumpState::Exit()
{
	//�ش� State �ִϸ��̼��� Reset��Ų��
	GetOwnerFSM()->GetOwner()->GetAnimator()->FindAnimation(L"BonusJump")->Reset();
}
