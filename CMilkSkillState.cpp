#include "pch.h"
#include "CMilkSkillState.h"
#include "CObj.h"
#include "CKeyManager.h"
#include "CTimeManager.h"
#include "CAnimator.h"
#include "CPlayer.h"
#include "CRigidbody.h"
#include "CAnimation.h"
#include "CCollider.h"
#include "CMilkCookie.h"
#include "CBackgroundEffect.h"
#include "CMilkShield.h"
#include "CMilkMonster.h"
#include "CCollisionManager.h"
#include "CLevelManager.h"
#include "CLevel.h"
#include "CSound.h"
#include <time.h>

CMilkSkillState::CMilkSkillState()
	:m_fAccTime(0)
{
}

CMilkSkillState::~CMilkSkillState()
{
}

void CMilkSkillState::final_tick()
{
	//� ��Ű���� �˾Ƴ���.
	CMilkCookie* pCookie = dynamic_cast<CMilkCookie*>(GetOwnerFSM()->GetOwner());

	//���� ������ ������ ��Ű�� �ƴ϶�� ����ó��
	assert(pCookie);

	//��ų ��� ���� �浹ü ����
	CCollisionManager::GetInst()->Clear();
	CCollisionManager::GetInst()->LayerCheck(LAYER::PLAYER, LAYER::JELLY);
	CCollisionManager::GetInst()->LayerCheck(LAYER::PLAYER_SKILL, LAYER::MONSTER);

	//============MilkSkillState���� ������ ��
	pCookie->SetOnSummon(true);

	//��Ű �ִϸ��̼�
	if (IsPressed(KEY::SPACE))
	{
		Vec2 V = pCookie->GetRigidbody()->GetVelocity();
		pCookie->GetRigidbody()->AddVelocity(Vec2(0.f, -(V.y + 300.f)));
		pCookie->GetAnimator()->Play(L"SkillUp", true);
		return;
	}
	if (IsRelease(KEY::SPACE))
	{
		pCookie->GetAnimator()->Play(L"SkillDown", false);
		return;
	}

	//�ð� ����
	m_fAccTime += DT;

	//�����õ� ����
	srand((unsigned)time(NULL));
	rand();

	//7������ ���͸� ������ų ����(0.9~1.8) 7���� ���Ѵ�.
	if (pCookie->GetSkillUsingAccTime() < pCookie->GetSkillMaxTime() - 10.f)
	{
		if (m_vecDuration.empty())
		{
			for (int i = 0; i < 7; i++)
			{
				float f = ((rand() % 9) + 9) / 10.f;
				m_vecDuration.push_back(f);
			}
		}
	}
	if (pCookie->GetSkillUsingAccTime() < pCookie->GetSkillMaxTime() - 2.f && pCookie->GetSkillUsingAccTime() != 0)
	{
		//������ y ��ǥ(100~620) ���� ���͸� �����Ѵ�.
		Vec2 vPos = Vec2(pCookie->GetPos().x + 1000.f, 360.f);

		for (size_t i = m_vecDuration.size() - 1; i >= 0; i--)
		{
			if (m_vecDuration.empty())
				break;

			if (m_fAccTime < m_vecDuration[i])
				break;

			if (m_fAccTime >= m_vecDuration[i])
			{
				if (i == 6 || i == 3)
				{
					//ū ���� ����
					CMilkMonster* pBigMonster = new CMilkMonster(MonsterSize::BIG);
					vPos.y = (rand() % 500) + 100;
					Instantiate(pBigMonster, vPos, LAYER::MONSTER);
				}

				else
				{
					//���� ���� ����
					CMilkMonster* pSmallMonster = new CMilkMonster(MonsterSize::SMALL);
					vPos.y = (rand() % 500) + 100;
					Instantiate(pSmallMonster, vPos, LAYER::MONSTER);
				}

				//���� �� �ʱ�ȭ
				m_fAccTime = 0;
				m_vecDuration.pop_back();
				return;
			}
		}
	}
	

	//���� �ִϸ��̼�
	if (pCookie->GetShield()->GetAnimator()->GetCurAnimation()->IsFinish())
	{
		pCookie->GetShield()->GetAnimator()->Play(L"SkillIdle", true);
	}

	//=============MilkSkillState�� ��ȯ ����

	//��ų ������� �ƴ϶��
	if (pCookie->IsOnSkill() == false)
	{
		GetOwnerFSM()->ChangeState(L"Run");
		return;
	}
}

void CMilkSkillState::Enter()
{
	//� ��Ű���� �˾Ƴ���.
	CMilkCookie* pCookie = dynamic_cast<CMilkCookie*>(GetOwnerFSM()->GetOwner());

	//�� off
	pCookie->GetRigidbody()->SetGround(false);

	//��Ű�� �߷� ��� ����
	pCookie->GetRigidbody()->SetGravityAccel(600.f);

	//������ ��׶����÷�
	m_pEffect = new CBackgroundEffect;
	m_pEffect->init(BACKGROUND_EFFECT::SEMITRANSPARENT);
	m_pEffect->SetAlphaReduce(false);
	Instantiate(m_pEffect, Vec2(0.f, 0.f), LAYER::BACKGROUND_EFFECT);

	//���� ����
	m_pMilkShield = new CMilkShield;
	Instantiate(m_pMilkShield, pCookie->GetPos(), LAYER::PLAYER_SKILL);
	m_pMilkShield->GetAnimator()->Play(L"SkillEnter", false);
	dynamic_cast<CMilkCookie*>(pCookie)->SetShield(m_pMilkShield);

	//SkillEnter �ִϸ��̼��� ����Ѵ�
	pCookie->GetAnimator()->Play(L"SkillEnter", false);

	//SkillEnter ȿ������ ����Ѵ�.
	pCookie->GetSkillEnterSFX()->Play();

	//����-���� �浹���� �߰�
	ResetCollision();
	CCollisionManager::GetInst()->LayerCheck(LAYER::PLAYER_SKILL, LAYER::MONSTER);
}

void CMilkSkillState::Exit()
{
	//� ��Ű���� �˾Ƴ���.
	CPlayer* pCookie = dynamic_cast<CPlayer*>(GetOwnerFSM()->GetOwner());

	//��Ű�� �߷� �ٽ� �ۿ�
	pCookie->GetRigidbody()->SetGravityAccel(800.f);

	//��׶���, ���� Exit �ִϸ��̼� ���
	m_pEffect->SetDead();
	m_pMilkShield->GetAnimator()->Play(L"SkillExit",false);

	//���� Ȱ��ȭ
	pCookie->SetInvincible(true);

	//SkillExit �ִϸ��̼��� ����Ѵ�
	pCookie->GetAnimator()->Play(L"SkillExit", false);
	
	//�ش� State���� ����� ��� �ִϸ��̼��� Reset��Ų��
	GetOwnerFSM()->GetOwner()->GetAnimator()->FindAnimation(L"SkillEnter")->Reset();
	GetOwnerFSM()->GetOwner()->GetAnimator()->FindAnimation(L"SkillUp")->Reset();
	GetOwnerFSM()->GetOwner()->GetAnimator()->FindAnimation(L"SkillDown")->Reset();
	GetOwnerFSM()->GetOwner()->GetAnimator()->FindAnimation(L"SkillExit")->Reset();

	//���� ���Ͱ� �ִٸ� Dead �ִϸ��̼� ��� �� Dead��Ŵ
	vector<CObj*> vecMonsterLayer = CLevelManager::GetInst()->GetCurLevel()->GetLayer(LAYER::MONSTER);
	for (int i = 0; i < vecMonsterLayer.size(); i++)
	{
		CMilkMonster* pMon = static_cast<CMilkMonster*>(vecMonsterLayer[i]);
		pMon->GetAnimator()->Play(L"MonsterDead",false);
	}

	pCookie->SetOnSummon(false);
	
	//�浹ü ���� ����
	ResetCollision();
}
