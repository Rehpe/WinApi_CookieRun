#include "pch.h"
#include "CBonusIdleState.h"
#include "CObj.h"
#include "CKeyManager.h"
#include "CTimeManager.h"
#include "CAnimator.h"
#include "CPlayer.h"
#include "CAnimation.h"
#include "CRigidbody.h"
#include "CLevelManager.h"
#include "CBonusStage.h"

CBonusIdleState::CBonusIdleState()
{
}

CBonusIdleState::~CBonusIdleState()
{
}

void CBonusIdleState::final_tick()
{
	//� ��Ű���� �˾Ƴ���.
	CPlayer* pCookie = dynamic_cast<CPlayer*>(GetOwnerFSM()->GetOwner());

	//���� ������ �÷��̾�(��Ű)�� �ƴ϶�� ����ó��
	assert(pCookie);

	//===========BonusIdleState���� ������ ��
	

	//=============BonusIdleState�� ��ȯ ����

	if (IsPressed(KEY::SPACE))
	{
		Vec2 V = pCookie->GetRigidbody()->GetVelocity();
		pCookie->GetRigidbody()->AddVelocity(Vec2(0.f, -(V.y + 300.f)));
		GetOwnerFSM()->ChangeState(L"BonusJump");
		return;
	}

}

void CBonusIdleState::Enter()
{
	//� ��Ű���� �˾Ƴ���.
	CPlayer* pCookie = dynamic_cast<CPlayer*>(GetOwnerFSM()->GetOwner());

	//BonusIdle �ִϸ��̼��� ����Ѵ�
	pCookie->GetAnimator()->Play(L"BonusIdle", true);
}

void CBonusIdleState::Exit()
{
	//�ش� State �ִϸ��̼��� Reset��Ų��
	GetOwnerFSM()->GetOwner()->GetAnimator()->FindAnimation(L"BonusIdle")->Reset();
}
