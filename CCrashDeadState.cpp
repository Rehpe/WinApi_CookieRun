#include "pch.h"
#include "CCrashDeadState.h"
#include "CObj.h"
#include "CKeyManager.h"
#include "CTimeManager.h"
#include "CAnimator.h"
#include "CPlayer.h"
#include "CAnimation.h"
#include "CRigidbody.h"
#include "CSound.h"

CCrashDeadState::CCrashDeadState()
{
}

CCrashDeadState::~CCrashDeadState()
{
}

void CCrashDeadState::final_tick()
{
	//� ��Ű���� �˾Ƴ���.
	CPlayer* pCookie = dynamic_cast<CPlayer*>(GetOwnerFSM()->GetOwner());

	//���� ������ �÷��̾�(��Ű)�� �ƴ϶�� ����ó��
	assert(pCookie);

	//===========CrashDeadState���� ������ ��
	pCookie->SetHP(0.f);
	pCookie->SetCrashed(true);

	//�����ð�(�� 1��) �� Game Over
	//������ Crash Animation�� �����߸� ��ȯ ����
	if (GetOwnerFSM()->GetOwner()->GetAnimator()->FindAnimation(L"CrashDead")->IsFinish())
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

			pCookie->SetCrashed(false);
			pCookie->SetPlayerDead(true);
			pCookie->SetSpeed(0.f);
			pCookie->GetGameOverSFX()->Play();
			ChangeLevel(LEVEL_TYPE::RESULT);
		}
	}
}

void CCrashDeadState::Enter()
{
	//� ��Ű���� �˾Ƴ���.
	CPlayer* pCookie = dynamic_cast<CPlayer*>(GetOwnerFSM()->GetOwner());

	//Crash �ִϸ��̼��� ����Ѵ�
	pCookie->GetAnimator()->Play(L"CrashDead", false);
}

void CCrashDeadState::Exit()
{
	//�ش� State �ִϸ��̼��� Reset��Ų��
	GetOwnerFSM()->GetOwner()->GetAnimator()->FindAnimation(L"CrashDead")->Reset();
}
