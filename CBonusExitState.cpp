#include "pch.h"
#include "CBonusExitState.h"
#include "CObj.h"
#include "CKeyManager.h"
#include "CTimeManager.h"
#include "CAnimator.h"
#include "CPlayer.h"
#include "CAnimation.h"
#include "CRigidbody.h"
#include "CLevelManager.h"
#include "CStage1.h"
#include "CEditorLevel.h"
#include "CCamera.h"
#include "CUI.h"

CBonusExitState::CBonusExitState()
{
}

CBonusExitState::~CBonusExitState()
{
}

void CBonusExitState::final_tick()
{
	//� ��Ű���� �˾Ƴ���.
	CPlayer* pCookie = dynamic_cast<CPlayer*>(GetOwnerFSM()->GetOwner());

	//���� ������ �÷��̾�(��Ű)�� �ƴ϶�� ����ó��
	assert(pCookie);

	//===========CBonusExitState���� ������ ��
	static float fDelay = 0;
	fDelay += DT;

	//=============CBonusExitState�� ��ȯ ����

	//������ Bonus Exit Animation�� �����߸� ��ȯ ����
	if (fDelay >=1.5f)
	{
		GetOwnerFSM()->ChangeState(L"Run");
		fDelay = 0;
	}
}

void CBonusExitState::Enter()
{
	//� ��Ű���� �˾Ƴ���.
	CPlayer* pCookie = dynamic_cast<CPlayer*>(GetOwnerFSM()->GetOwner());

	//��Ű�� �߷� �ٽ� ���󺹱�
	pCookie->GetRigidbody()->SetGravityAccel(800.f);

	//BonusExit �ִϸ��̼��� ����Ѵ�
	pCookie->GetAnimator()->Play(L"BonusExit", false);

}

void CBonusExitState::Exit()
{
	//� ��Ű���� �˾Ƴ���.
	CPlayer* pCookie = dynamic_cast<CPlayer*>(GetOwnerFSM()->GetOwner());

	//�ش� State �ִϸ��̼��� Reset��Ų��
	GetOwnerFSM()->GetOwner()->GetAnimator()->FindAnimation(L"BonusExit")->Reset();

}
