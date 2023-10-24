#include "pch.h"
#include "CPetBonusEnterState.h"
#include "CObj.h"
#include "CFSM.h"
#include "CAnimation.h"
#include "CAnimator.h"
#include "CPet.h"
#include "CLevelManager.h"
#include "CLevel.h"
#include "CPlayer.h"

CPetBonusEnterState::CPetBonusEnterState()
{
}

CPetBonusEnterState::~CPetBonusEnterState()
{
}

void CPetBonusEnterState::final_tick()
{
	//============BonusEnterState���� ������ ��

	CPet* pPet = dynamic_cast<CPet*>(GetOwnerFSM()->GetOwner());
	assert(pPet);

	////��Ű�� ��ġ ��������
	//CPlayer* pCookie = CLevelManager::GetInst()->GetCurLevel()->GetPlayer();

	//���� ���ʽ� �������� ���� �� �÷��̾��� ���ʿ� �׻� �پ��ִ´�.
	Vec2 vPos = pPet->GetPlayer()->GetPos() - Vec2(0.f, 100.f);
	pPet->SetPos(vPos);

	//=============BonusEnterState�� ��ȯ ����

	//������ BonusEnter Animation�� �����߸� ��ȯ ����
	if (GetOwnerFSM()->GetOwner()->GetAnimator()->FindAnimation(L"BonusEnter")->IsFinish())
	{
		GetOwnerFSM()->ChangeState(L"BonusIdle");
		return;
	}
}

void CPetBonusEnterState::Enter()
{
	//� ������ �˾Ƴ���.
	CPet* pPet = dynamic_cast<CPet*>(GetOwnerFSM()->GetOwner());

	//���� �ƴ϶�� ����ó��
	assert(pPet);

	//BonusEnter �ִϸ��̼��� ����Ѵ�
	GetOwnerFSM()->GetOwner()->GetAnimator()->Play(L"BonusEnter", false);
}

void CPetBonusEnterState::Exit()
{
	//�ش� State �ִϸ��̼��� Reset��Ų��
	GetOwnerFSM()->GetOwner()->GetAnimator()->FindAnimation(L"BonusEnter")->Reset();
}
