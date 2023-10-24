#include "pch.h"
#include "CPetBonusExitState.h"
#include "CObj.h"
#include "CFSM.h"
#include "CAnimation.h"
#include "CAnimator.h"
#include "CPet.h"
#include "CLevelManager.h"
#include "CLevel.h"
#include "CPlayer.h"

CPetBonusExitState::CPetBonusExitState()
{
}

CPetBonusExitState::~CPetBonusExitState()
{
}

void CPetBonusExitState::final_tick()
{
	//============BonusExitState���� ������ ��

	CPet* pPet = dynamic_cast<CPet*>(GetOwnerFSM()->GetOwner());
	assert(pPet);

	//��Ű�� ��ġ ��������
	CPlayer* pCookie = CLevelManager::GetInst()->GetCurLevel()->GetPlayer();

	//���� ���ʽ� �������� ���� �� �÷��̾��� ���ʿ� �׻� �پ��ִ´�.
	Vec2 vPos = pCookie->GetPos() - Vec2(0.f, 130.f);



	pPet->SetPos(vPos);

	//=============BonusExitState�� ��ȯ ����

	//������ BonusExit Animation�� �����߸� ��ȯ ����
	if (GetOwnerFSM()->GetOwner()->GetAnimator()->FindAnimation(L"BonusExit")->IsFinish())
	{
		GetOwnerFSM()->ChangeState(L"Idle");
		return;
	}
}

void CPetBonusExitState::Enter()
{
	//� ������ �˾Ƴ���.
	CPet* pPet = dynamic_cast<CPet*>(GetOwnerFSM()->GetOwner());

	//���� �ƴ϶�� ����ó��
	assert(pPet);

	//BonusExit �ִϸ��̼��� ����Ѵ�
	GetOwnerFSM()->GetOwner()->GetAnimator()->Play(L"BonusExit", false);
}

void CPetBonusExitState::Exit()
{
	//�ش� State �ִϸ��̼��� Reset��Ų��
	GetOwnerFSM()->GetOwner()->GetAnimator()->FindAnimation(L"BonusExit")->Reset();
}
