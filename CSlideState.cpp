#include "pch.h"
#include "CSlideState.h"
#include "CObj.h"
#include "CKeyManager.h"
#include "CTimeManager.h"
#include "CAnimator.h"
#include "CPlayer.h"
#include "CAnimation.h"
#include "CCollider.h"
#include "CMilkCookie.h"
#include "CGingerBraveCookie.h"
#include "CSound.h"

CSlideState::CSlideState()
{
}

CSlideState::~CSlideState()
{
}

void CSlideState::final_tick()
{
	//� ��Ű���� �˾Ƴ���.
	CPlayer* pCookie = dynamic_cast<CPlayer*>(GetOwnerFSM()->GetOwner());

	//���� ������ �÷��̾�(��Ű)�� �ƴ϶�� ����ó��
	assert(pCookie);


	//============SlideState���� ������ ��

	//���� ��Ű�� �浹ü�� �����ϰ� �����Ѵ�
	CCollider* pCollider = pCookie->GetCollider();
	pCollider->SetOffsetScale(Vec2(0.f, -60.f)); //�浹ü ũ�� 100, 65
	pCollider->SetOffsetPos(Vec2(0.f, 30.f));

	//�ν��� ������ ��� �浹ü �������� �ణ �����Ѵ�
	if (pCookie->IsBigger() == true)
	{
		pCollider->SetOffsetScale(Vec2(0.f,-120.f));
		pCollider->SetOffsetPos(Vec2(0.f, 60.f));
	}
	
	//=============RunState�� ��ȯ ����

	//�����̵�Ű�� ���� Run���� ����
	if (IsRelease(KEY::DOWN))
	{
		GetOwnerFSM()->ChangeState(L"Run");
		return;
	}

	if (IsTap(KEY::SPACE))
	{
		GetOwnerFSM()->ChangeState(L"Jump");
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
		return;
	}

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

		else
		{
			return;
		}
	}
	//�ν��͸� �Ծ��ٸ�
	//���ʽ�Ÿ�� ������ �����ߴٸ�
}

void CSlideState::Enter()
{
	//� ��Ű���� �˾Ƴ���.
	CPlayer* pCookie = dynamic_cast<CPlayer*>(GetOwnerFSM()->GetOwner());

	pCookie->SetJump(false);

	//Slide �ִϸ��̼��� ����Ѵ�
	pCookie->GetAnimator()->Play(L"Slide", true);

	//Slide ȿ������ ����Ѵ�.
	pCookie->GetSlideSFX()->Play();

	//���� ��Ű�� �浹ü�� �����ϰ� �����Ѵ�
	CCollider* pCollider = pCookie->GetCollider();
	//pCollider->SetOffsetScale(Vec2(0.f, -30.f)); //�浹ü ũ�� 100, 65
	//pCollider->SetOffsetPos(Vec2(0.f, -30.f));
}

void CSlideState::Exit()
{
	//� ��Ű���� �˾Ƴ���.
	CPlayer* pCookie = dynamic_cast<CPlayer*>(GetOwnerFSM()->GetOwner());

	//�ش� State �ִϸ��̼��� Reset��Ų��
	pCookie->GetAnimator()->FindAnimation(L"Slide")->Reset();

	//�پ�� �浹ü ũ�⸦ ���󺹱���Ų��
	pCookie->GetCollider()->SetOffsetScale(Vec2(0.f, 0.f));	//�浹ü ũ�� 100, 125
	pCookie->GetCollider()->SetOffsetPos(Vec2(0.f, 0.f));

}
