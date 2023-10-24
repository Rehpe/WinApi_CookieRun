#include "pch.h"
#include "CDoubleJumpState.h"
#include "CObj.h"
#include "CKeyManager.h"
#include "CTimeManager.h"
#include "CAnimator.h"
#include "CPlayer.h"
#include "CAnimation.h"
#include "CRigidbody.h"
#include "CCamera.h"
#include "CSound.h"

CDoubleJumpState::CDoubleJumpState()
{
}

CDoubleJumpState::~CDoubleJumpState()
{
}

void CDoubleJumpState::final_tick()
{
	//� ��Ű���� �˾Ƴ���.
	CPlayer* pCookie = dynamic_cast<CPlayer*>(GetOwnerFSM()->GetOwner());

	//���� ������ �÷��̾�(��Ű)�� �ƴ϶�� ����ó��
	assert(pCookie);

	//===========CrashState���� ������ ��


	pCookie->SetJump(true);



	//=============DoubleJumpState�� ��ȯ ����

	//���� ��ư�� ������ && ��Ű�� ���� �������̰� && ��Ű�� ���� ����Ƚ���� 0���� ũ�ٸ�
	if (IsTap(KEY::SPACE) && pCookie->IsJump() && pCookie->GetJumpCount() > 0)
	{
		Vec2 V = pCookie->GetRigidbody()->GetVelocity();
		pCookie->GetRigidbody()->AddVelocity(Vec2(0.f, -(V.y + 600.f)));
		GetOwnerFSM()->ChangeState(L"DoubleJump");
		return;
	}

	//��Ű�� ���� ���� ���¸�
	if (pCookie->GetRigidbody()->IsGround())
	{
		pCookie->SetJump(false);
		GetOwnerFSM()->ChangeState(L"Run");
		//��ų ������� �������� ��Ű��� Skill���·� ���ư���
		if (pCookie->GetName() == L"SnowSugarCookie" && pCookie->IsOnSkill())
		{
			pCookie->GetFSM()->ChangeState(L"Skill");
		}

		//�Ŵ�ȭ ���¿��� Jump->Run ���·� ���ƿ��� ī�޶� ��鸲 + ���� ȿ���� ���
		if (pCookie->IsBigger() || (pCookie->GetName() == L"SnowSugarCookie" && pCookie->IsOnSkill()))
		{
			CCamera::GetInst()->CameraShake(5.f, 300.f, 0.3f);
			pCookie->GetBiggerLandSFX()->Play();
		}
		return;
	}

	if (pCookie->IsCrashed())
	{
		GetOwnerFSM()->ChangeState(L"Crash");
		return;
	}
	//�ν���
	// 
	//���ʽ����ǿ� ����������
	if (pCookie->IsBonus())
	{
		if (pCookie->IsOnSummon())
			return;

		GetOwnerFSM()->ChangeState(L"BonusEnter");
		return;
	}
}

void CDoubleJumpState::Enter()
{
	//� ��Ű���� �˾Ƴ���.
	CPlayer* pCookie = dynamic_cast<CPlayer*>(GetOwnerFSM()->GetOwner());

	pCookie->SetJump(true);

	//��Ű�� ���� ī��Ʈ ����
	int iJumpCount = pCookie->GetJumpCount();
	pCookie->SetJumpCount(--iJumpCount);

	//DoubleJump �ִϸ��̼��� ����Ѵ�
	pCookie->GetAnimator()->Play(L"DoubleJump", true);
	//�������� ��Ű ��ų ���¶�� ���� �ִϸ��̼� ���
	if (pCookie->GetName() == L"SnowSugarCookie" && pCookie->IsOnSkill())
	{
		pCookie->GetAnimator()->Play(L"SkillJump", false);
	}

	//Jump ȿ������ ����Ѵ�.
	pCookie->GetJumpSFX()->Play();
}

void CDoubleJumpState::Exit()
{
	//� ��Ű���� �˾Ƴ���.
	CPlayer* pCookie = dynamic_cast<CPlayer*>(GetOwnerFSM()->GetOwner());

	//�ش� State �ִϸ��̼��� Reset��Ų��
	GetOwnerFSM()->GetOwner()->GetAnimator()->FindAnimation(L"DoubleJump")->Reset();
	//��ų ������� �������� ��Ű��� ���� �ִϸ��̼� Reset
	if (pCookie->GetName() == L"SnowSugarCookie" && pCookie->IsOnSkill())
	{
		pCookie->GetAnimator()->FindAnimation(L"SkillJump")->Reset();
	}
}
