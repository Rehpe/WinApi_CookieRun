#include "pch.h"
#include "CCrashState.h"
#include "CObj.h"
#include "CKeyManager.h"
#include "CTimeManager.h"
#include "CAnimator.h"
#include "CPlayer.h"
#include "CAnimation.h"
#include "CRigidbody.h"
#include "CCamera.h"
#include "CSound.h"

CCrashState::CCrashState()
{
}

CCrashState::~CCrashState()
{
}

void CCrashState::final_tick()
{
	//� ��Ű���� �˾Ƴ���.
	CPlayer* pCookie = dynamic_cast<CPlayer*>(GetOwnerFSM()->GetOwner());

	//���� ������ �÷��̾�(��Ű)�� �ƴ϶�� ����ó��
	assert(pCookie);

	//===========CrashState���� ������ ��
	pCookie->SetCrashed(true);



	//=============CrashState�� ��ȯ ����

	//������ Crash Animation�� �����߸� ��ȯ ����
	if (GetOwnerFSM()->GetOwner()->GetAnimator()->FindAnimation(L"Crash")->IsFinish())
	{
		//��Ű�� �ε�ģ ���� && HP�� 0���� ���ų� ���ٸ�
		if (pCookie->IsCrashed() && pCookie->GetHP() <=0)
		{
			GetOwnerFSM()->ChangeState(L"CrashDead");
			return;
		}

		else if (pCookie->GetHP() > 0 && IsPressed(KEY::DOWN))
		{
			pCookie->SetCrashed(false);
			GetOwnerFSM()->ChangeState(L"Slide");
			return;
		}

		//�� ���°� �ƴ϶�� Run���·� ����
		else if (pCookie->GetHP() > 0)
		{
			pCookie->SetCrashed(false);
			GetOwnerFSM()->ChangeState(L"Run");
			return;
		}

		//�ν���
		//���ʽ�Ÿ�� ������ �����ߴٸ�
		if (pCookie->IsBonus())
		{
			GetOwnerFSM()->ChangeState(L"BonusEnter");
			return;
		}
	}
}

void CCrashState::Enter()
{
	//� ��Ű���� �˾Ƴ���.
	CPlayer* pCookie = dynamic_cast<CPlayer*>(GetOwnerFSM()->GetOwner());

	//��Ű �������� ������ true�� �����.
	pCookie->SetInvincible(true);

	//ī�޶� shake�� �ش�
	CCamera::GetInst()->CameraShake(5.f, 300.f, 0.5f);

	//Crash �ִϸ��̼��� ����Ѵ�
	pCookie->GetAnimator()->Play(L"Crash", false);

	//Crash ȿ������ ����Ѵ�
	pCookie->GetCrashSFX()->Play();
}

void CCrashState::Exit()
{
	//�ش� State �ִϸ��̼��� Reset��Ų��
	GetOwnerFSM()->GetOwner()->GetAnimator()->FindAnimation(L"Crash")->Reset();
}
