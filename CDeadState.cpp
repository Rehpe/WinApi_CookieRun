#include "pch.h"
#include "CDeadState.h"
#include "CObj.h"
#include "CKeyManager.h"
#include "CTimeManager.h"
#include "CAnimator.h"
#include "CPlayer.h"
#include "CAnimation.h"
#include "CRigidbody.h"
#include "CCamera.h"
#include "CLevelManager.h"
#include "CLevel.h"
#include "CResultLevel.h"
#include "CSound.h"

CDeadState::CDeadState()
{
}

CDeadState::~CDeadState()
{
}

void CDeadState::final_tick()
{
	//� ��Ű���� �˾Ƴ���.
	CPlayer* pCookie = dynamic_cast<CPlayer*>(GetOwnerFSM()->GetOwner());

	//���� ������ �÷��̾�(��Ű)�� �ƴ϶�� ����ó��
	assert(pCookie);

	//===========DeadState���� ������ ��
	pCookie->SetHP(0.f);

	//=============DeadState�� ��ȯ ����

	//������ Crash Animation�� �����߸� ��ȯ ����
	if (GetOwnerFSM()->GetOwner()->GetAnimator()->FindAnimation(L"Dead")->IsFinish())
	{
		pCookie->SetSpeed(0.f);
		static float fAccTime = 0;
		fAccTime += DT;

		//1�� �Ŀ�
		if (fAccTime >= 1)
		{
			//��Ȱ ����� �ִٸ� ��Ȱ

			//���ٸ� ���� ���â����

			fAccTime = 0;

			pCookie->SetPlayerDead(true);
			pCookie->SetSpeed(0.f);
			pCookie->GetGameOverSFX()->Play();
			ChangeLevel(LEVEL_TYPE::RESULT);
		}
	}
}

void CDeadState::Enter()
{
	//� ��Ű���� �˾Ƴ���.
	CPlayer* pCookie = dynamic_cast<CPlayer*>(GetOwnerFSM()->GetOwner());

	//Dead �ִϸ��̼��� ����Ѵ�
	pCookie->GetAnimator()->Play(L"Dead", false);
}

void CDeadState::Exit()
{
	//�ش� State �ִϸ��̼��� Reset��Ų��
	GetOwnerFSM()->GetOwner()->GetAnimator()->FindAnimation(L"Dead")->Reset();
}
