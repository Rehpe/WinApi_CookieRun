#include "pch.h"
#include "CBonusJumpState.h"
#include "CObj.h"
#include "CKeyManager.h"
#include "CTimeManager.h"
#include "CAnimator.h"
#include "CPlayer.h"
#include "CAnimation.h"
#include "CRigidbody.h"
#include "CLevelManager.h"
#include "CBonusStage.h"

CBonusJumpState::CBonusJumpState()
{
}

CBonusJumpState::~CBonusJumpState()
{
}

void CBonusJumpState::final_tick()
{
	//� ��Ű���� �˾Ƴ���.
	CPlayer* pCookie = dynamic_cast<CPlayer*>(GetOwnerFSM()->GetOwner());

	//���� ������ �÷��̾�(��Ű)�� �ƴ϶�� ����ó��
	assert(pCookie);

	//===========BonusJumpState���� ������ ��


	//=============BonusJumpState�� ��ȯ ����

	if (IsRelease(KEY::SPACE))
	{
		GetOwnerFSM()->ChangeState(L"BonusIdle");
		return;
	}
	//if (!pCookie->IsBonus())
	//{
	//	GetOwnerFSM()->ChangeState(L"BonusExit");
	//	return;
	//}
}

void CBonusJumpState::Enter()
{
	//� ��Ű���� �˾Ƴ���.
	CPlayer* pCookie = dynamic_cast<CPlayer*>(GetOwnerFSM()->GetOwner());

	//BonusJump �ִϸ��̼��� ����Ѵ�
	pCookie->GetAnimator()->Play(L"BonusJump", true);
}

void CBonusJumpState::Exit()
{
	//�ش� State �ִϸ��̼��� Reset��Ų��
	GetOwnerFSM()->GetOwner()->GetAnimator()->FindAnimation(L"BonusJump")->Reset();
}
