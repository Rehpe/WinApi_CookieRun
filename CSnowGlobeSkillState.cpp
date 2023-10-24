#include "pch.h"
#include "CSnowGlobeSkillState.h"
#include "CPet.h"
#include "CAnimator.h"
#include "CAnimation.h"
#include "CTimeManager.h"
#include "CPetSnowGlobe.h"
#include "CLevelManager.h"
#include "CLevel.h"
#include "CPlayer.h"
#include "CJelly.h"
#include "CSound.h"

CSnowGlobeSkillState::CSnowGlobeSkillState()
	:m_fDelay(0)
{
}

CSnowGlobeSkillState::~CSnowGlobeSkillState()
{
}

void CSnowGlobeSkillState::final_tick()
{
	CPetSnowGlobe* pPet = dynamic_cast<CPetSnowGlobe*>(GetOwnerFSM()->GetOwner());
	assert(pPet);

	CPlayer* pCookie = CLevelManager::GetInst()->GetCurLevel()->GetPlayer();

	//���� Ÿ�� ����Ʈ�� ������
	Vec2 vTargetPos = Vec2(pCookie->GetPos().x + 550.f, 350.f);
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

	//�� 0.5�� �ڿ� ��������Ƽ ������ �������� �����ϰ� ���ƿ´�.
	m_fDelay += DT;

	if (m_fDelay >= 0.5f)
	{
		m_fDelay = 0;

		CJelly* pJelly = new CJelly;
		pJelly->init(ITEM_TYPE::PETJELLY_SNOWSUGAR);
		Instantiate(pJelly, pPet->GetPos(), LAYER::JELLY);
		pPet->GetMakeJellySFX()->Play();

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

void CSnowGlobeSkillState::Enter()
{
	//� ������ �˾Ƴ���.
	CPet* pPet = dynamic_cast<CPet*>(GetOwnerFSM()->GetOwner());

	//���� �ƴ϶�� ����ó��
	assert(pPet);

	//Skill �ִϸ��̼��� ����Ѵ�
	pPet->GetAnimator()->Play(L"Skill", true);
}

void CSnowGlobeSkillState::Exit()
{
	//�ش� State �ִϸ��̼��� Reset��Ų��
	GetOwnerFSM()->GetOwner()->GetAnimator()->FindAnimation(L"Skill")->Reset();
}
