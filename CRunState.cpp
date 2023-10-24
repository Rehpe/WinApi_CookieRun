#include "pch.h"
#include "CRunState.h"
#include "CObj.h"
#include "CKeyManager.h"
#include "CTimeManager.h"
#include "CAnimator.h"
#include "CPlayer.h"
#include "CAnimation.h"
#include "CRigidbody.h"
#include "CMilkCookie.h"
#include "CSnowSugarCookie.h"

CRunState::CRunState()
{
}

CRunState::~CRunState()
{
}

void CRunState::final_tick()
{
	//� ��Ű���� �˾Ƴ���.
	CPlayer* pCookie = dynamic_cast<CPlayer*>(GetOwnerFSM()->GetOwner());
	
	//���� ������ �÷��̾�(��Ű)�� �ƴ϶�� ����ó��
	assert(pCookie);

	
	//============RunState���� ������ ��

	//���� ��Ű�� ��ġ ��������
	Vec2 vPos = pCookie->GetPos();

	//GetRigidbody()->AddForce(Vec2(300.f, 0.f)) ;

	if (IsPressed(KEY::LEFT))
	{
		vPos += Vec2(-700.f, 0.f) * DT;
	}
	if (IsPressed(KEY::RIGHT))
	{
		vPos += Vec2(700.f, 0.f) * DT;
	}

	pCookie->SetPos(vPos);


	//=============RunState�� ��ȯ ����

	if (IsTap(KEY::SPACE))
	{
		//� ��Ű���� �˾Ƴ���.
		CPlayer* pCookie = dynamic_cast<CPlayer*>(GetOwnerFSM()->GetOwner());

		pCookie->GetRigidbody()->AddVelocity(Vec2(0.f, -600.f));
		pCookie->GetRigidbody()->SetGround(false);
		GetOwnerFSM()->ChangeState(L"Jump");
		return;
	}

	if (IsPressed(KEY::DOWN))
	{
		GetOwnerFSM()->ChangeState(L"Slide");
		return;
	}

	if (pCookie->IsCrashed())
	{
		GetOwnerFSM()->ChangeState(L"Crash");
		return;
	}

	if (pCookie->GetHP() <= 0.f)
	{
		GetOwnerFSM()->ChangeState(L"Dead");
	}
	
	//�ν��͸� �Ծ��ٸ�
	
	//���ʽ�Ÿ�� ������ �����ߴٸ�
	if (pCookie->IsBonus())
	{
		if (pCookie->IsOnSummon())
			return;

		GetOwnerFSM()->ChangeState(L"BonusEnter");
		return;
	}

	//��ų ���
	if (pCookie->IsOnSkill())
	{
		//������ ��Ű���
		pCookie = dynamic_cast<CMilkCookie*>(pCookie);
		if (pCookie != nullptr)
		{
			GetOwnerFSM()->ChangeState(L"Skill");
			return;
		}
		pCookie = dynamic_cast<CSnowSugarCookie*>(pCookie);
		if (pCookie != nullptr)
		{
			GetOwnerFSM()->ChangeState(L"SkillEnter");
			return;
		}

		else
		{
			return;
		}
	}
}

void CRunState::Enter()
{
	//� ��Ű���� �˾Ƴ���.
	CPlayer* pCookie = dynamic_cast<CPlayer*>(GetOwnerFSM()->GetOwner());

	pCookie->SetJump(false);

	//��Ű�� ���� ī��Ʈ 2�� ����
	int iJumpCount = pCookie->GetJumpCount();
	pCookie->SetJumpCount(2);

	if (pCookie->IsBooster())
	{//Booster���¸� Booster�ִϸ��̼��� ����Ѵ�
		GetOwnerFSM()->GetOwner()->GetAnimator()->Play(L"Boost", true);
		return;
	}
	//Run �ִϸ��̼��� ����Ѵ�
	GetOwnerFSM()->GetOwner()->GetAnimator()->Play(L"Run", true);


}

void CRunState::Exit()
{
	//�ش� State �ִϸ��̼��� Reset��Ų��
	GetOwnerFSM()->GetOwner()->GetAnimator()->FindAnimation(L"Run")->Reset();
}
