#include "pch.h"
#include "CSnowSugarSkillState.h"
#include "CSnowSugarCookie.h"
#include "CKeyManager.h"
#include "CRigidbody.h"
#include "CAnimator.h"
#include "CTimeManager.h"
#include "CAnimation.h"
#include "CSound.h"
#include "CCollider.h"
#include "CJelly.h"
#include <time.h>

CSnowSugarSkillState::CSnowSugarSkillState()
{
}

CSnowSugarSkillState::~CSnowSugarSkillState()
{
}

void CSnowSugarSkillState::final_tick()
{
	//� ��Ű���� �˾Ƴ���.
	CSnowSugarCookie* pCookie = dynamic_cast<CSnowSugarCookie*>(GetOwnerFSM()->GetOwner());

	//���� ������ ������ ��Ű�� �ƴ϶�� ����ó��
	assert(pCookie);

	//============SnowSugarSkill���� ������ ��
	pCookie->GetAnimator()->SetOffset(Vec2(0.f, 105.f));


	//�����õ� ����
	srand((unsigned)time(NULL));
	rand();

	//�÷��̾� �� x�࿡�� ������ y������ 0.5�ʸ��� ���������� �����Ѵ�.
	//������ ���������� ��ֹ��� ������ ��ֹ��� ������


	//=============SnowSugarSkill�� ��ȯ ����

	if (IsTap(KEY::SPACE))
	{
		//� ��Ű���� �˾Ƴ���.
		CPlayer* pCookie = dynamic_cast<CPlayer*>(GetOwnerFSM()->GetOwner());

		pCookie->GetRigidbody()->AddVelocity(Vec2(0.f, -600.f));
		pCookie->GetRigidbody()->SetGround(false);
		GetOwnerFSM()->ChangeState(L"Jump");
		return;
	}

	if (!pCookie->IsOnSkill())
	{
		GetOwnerFSM()->ChangeState(L"SkillExit");
		return;
	}
}

void CSnowSugarSkillState::Enter()
{
	//� ��Ű���� �˾Ƴ���.
	CSnowSugarCookie* pCookie = dynamic_cast<CSnowSugarCookie*>(GetOwnerFSM()->GetOwner());

	//SkillEnter �ִϸ��̼��� ����Ѵ�
	pCookie->GetAnimator()->Play(L"SkillIdle", false);

	pCookie->SetScale(Vec2(360.f, 300.f));


	////SkillEnter ȿ������ ����Ѵ�.
	//pCookie->GetSkillEnterSFX()->Play();
}

void CSnowSugarSkillState::Exit()
{
	//� ��Ű���� �˾Ƴ���.
	CSnowSugarCookie* pCookie = dynamic_cast<CSnowSugarCookie*>(GetOwnerFSM()->GetOwner());

	m_fDelay = 0;

	//�ش� State���� ����� ��� �ִϸ��̼��� Reset��Ų��
	pCookie->GetAnimator()->FindAnimation(L"SkillIdle")->Reset();

	////���� ���Ͱ� �ִٸ� Dead �ִϸ��̼� ��� �� Dead��Ŵ
	//vector<CObj*> vecMonsterLayer = CLevelManager::GetInst()->GetCurLevel()->GetLayer(LAYER::MONSTER);
	//for (int i = 0; i < vecMonsterLayer.size(); i++)
	//{
	//	CMilkMonster* pMon = static_cast<CMilkMonster*>(vecMonsterLayer[i]);
	//	pMon->GetAnimator()->Play(L"MonsterDead", false);
	//}
}
