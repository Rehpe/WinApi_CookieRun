#include "pch.h"
#include "CMilkBottleSkillState.h"
#include "CPet.h"
#include "CPlayer.h"
#include "CPetMilkBottle.h"
#include "CAnimation.h"
#include "CAnimator.h"
#include "CJelly.h"
#include "CLevelManager.h"
#include "CLevel.h"
#include "CTimeManager.h"
#include "CSound.h"

CMilkBottleSkillState::CMilkBottleSkillState()
	:m_fAccTime(0)
	, m_fDelay(0)
{
}

CMilkBottleSkillState::~CMilkBottleSkillState()
{
	if (!m_vecMilkJelly.empty())
	{
		for (size_t i = 0; i < m_vecMilkJelly.size(); i++)
		{
			DEL(m_vecMilkJelly[i]);
		}
		m_vecMilkJelly.clear();
	}
}

void CMilkBottleSkillState::final_tick()
{
	//============SkillState���� ������ ��

	CPetMilkBottle* pPet = dynamic_cast<CPetMilkBottle*>(GetOwnerFSM()->GetOwner());
	assert(pPet);

	CPlayer* pCookie = CLevelManager::GetInst()->GetCurLevel()->GetPlayer();

	//���� Ÿ�� ����Ʈ�� ������
	Vec2 vTargetPos = Vec2(pCookie->GetPos().x + 350.f, 350.f);
	Vec2 vPos = pPet->GetPos();

	Vec2 vDir = vTargetPos - vPos;
	vDir.Normalize();

	vPos.x += vDir.x * 1000.f * DT;
	vPos.y += vDir.y * 1000.f * DT;


	if (pCookie->GetPos().x + 198.f <= vPos.x && vPos.x <= pCookie->GetPos().x + 202.f
		&& pCookie->GetPos().y - 198.f <= vPos.y && vPos.y <= pCookie->GetPos().y - 202.f)
	{
		vPos = pCookie->GetPos() + Vec2(200.f, -200.f);
	}

	pPet->SetPos(vPos);

	//�� 0.7�� �ں��� ������ �������� ���� ��ġ�� 0.4�ʸ��� �����Ѵ�
	m_fDelay += DT;

 	if (m_fDelay >= 0.7f)
	{
		m_fAccTime += DT;

		if (m_fAccTime >= 0.4f)
		{
			if (m_vecMilkJelly.empty())
				return; 

			for (int i = m_vecMilkJelly.size()-1; i >=0; i--)
			{
				CJelly* pJelly = m_vecMilkJelly[i];
				Instantiate(pJelly, pPet->GetPos(), LAYER::JELLY);
				m_vecMilkJelly.pop_back();
				pPet->GetMakeMilkJellySFX()->Play();
				m_fAccTime = 0;
				break;
			}
		}
	}

	//���� ���� ��� ������ �����ߴٸ� ��ų ������� �����Ѵ�.
	if (m_vecMilkJelly.empty())
	{
		pPet->SetOnSkill(false);
	}

	//=============SkillState�� ��ȯ ����

	//��ų ��� ���°� �ƴ϶�� Idle�� ��ȯ
	if (!pPet->IsOnSkill())
	{
		GetOwnerFSM()->ChangeState(L"Idle");
		return;
	}
	
}

void CMilkBottleSkillState::Enter()
{
	//� ������ �˾Ƴ���.
	CPet* pPet = dynamic_cast<CPet*>(GetOwnerFSM()->GetOwner());

	//���� �ƴ϶�� ����ó��
	assert(pPet);

	//Skill �ִϸ��̼��� ����Ѵ�
	pPet->GetAnimator()->Play(L"Skill", true);

	//������ �������� 10���� ���Ϳ� ��Ƴ��´�
	for (int i = 0; i < 10; i++)
	{
		CJelly* pMilkJelly = new CJelly;
		pMilkJelly->init(ITEM_TYPE::PETJELLY_MILK);
		m_vecMilkJelly.push_back(pMilkJelly);
	}
}


void CMilkBottleSkillState::Exit()
{
	//�ش� State �ִϸ��̼��� Reset��Ų��
	GetOwnerFSM()->GetOwner()->GetAnimator()->FindAnimation(L"Skill")->Reset();
}
