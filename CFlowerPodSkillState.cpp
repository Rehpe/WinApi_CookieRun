#include "pch.h"
#include "CFlowerPodSkillState.h"
#include "CPet.h"
#include "CPlayer.h"
#include "CPetFlowerPod.h"
#include "CAnimation.h"
#include "CAnimator.h"
#include "CJelly.h"
#include "CLevelManager.h"
#include "CLevel.h"
#include "CTimeManager.h"
#include "CSound.h"

CFlowerPodSkillState::CFlowerPodSkillState()
	:m_fAccTime(0)
	, m_fDelay(0)
{
}

CFlowerPodSkillState::~CFlowerPodSkillState()
{
	if (!m_vecFlowerJelly.empty())
	{
		for (size_t i = 0; i < m_vecFlowerJelly.size(); i++)
		{
			DEL(m_vecFlowerJelly[i]);
		}
		m_vecFlowerJelly.clear();
	}
}

void CFlowerPodSkillState::final_tick()
{
	//============SkillState에서 실행할 것

	CPetFlowerPod* pPet = dynamic_cast<CPetFlowerPod*>(GetOwnerFSM()->GetOwner());
	assert(pPet);

	CPlayer* pCookie = CLevelManager::GetInst()->GetCurLevel()->GetPlayer();

	//펫이 타켓 포인트로 나간다
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

	//약 0.7초 뒤부터 벡터의 젤리들을 펫의 위치에 0.4초마다 생성한다
	m_fDelay += DT;

	if (m_fDelay >= 0.7f)
	{
		m_fAccTime += DT;

		if (m_fAccTime >= 0.4f)
		{
			if (m_vecFlowerJelly.empty())
				return;

			for (int i = m_vecFlowerJelly.size() - 1; i >= 0; i--)
			{
				CJelly* pJelly = m_vecFlowerJelly[i];
				Instantiate(pJelly, pPet->GetPos(), LAYER::JELLY);
				m_vecFlowerJelly.pop_back();
				pPet->GetMakeFlowerJellySFX()->Play();
				m_fAccTime = 0;
				break;
			}
		}
	}

	//벡터 안의 모든 젤리를 생성했다면 스킬 사용중을 해제한다.
	if (m_vecFlowerJelly.empty())
	{
		pPet->SetOnSkill(false);
	}

	//=============SkillState의 전환 조건

	//스킬 사용 상태가 아니라면 Idle로 전환
	if (!pPet->IsOnSkill())
	{
		GetOwnerFSM()->ChangeState(L"Idle");
		return;
	}

}

void CFlowerPodSkillState::Enter()
{
	//어떤 펫인지 알아낸다.
	CPet* pPet = dynamic_cast<CPet*>(GetOwnerFSM()->GetOwner());

	//펫이 아니라면 에러처리
	assert(pPet);

	//Skill 애니메이션을 재생한다
	pPet->GetAnimator()->Play(L"Skill", true);

	//생성할 꽃젤리 5개를 벡터에 담아놓는다
	for (int i = 0; i < 5; i++)
	{
		CJelly* pFlowerJelly = new CJelly;
		pFlowerJelly->init(ITEM_TYPE::PETJELLY_FAIRY);
		m_vecFlowerJelly.push_back(pFlowerJelly);
	}
}

void CFlowerPodSkillState::Exit()
{
	//해당 State 애니메이션을 Reset시킨다
	GetOwnerFSM()->GetOwner()->GetAnimator()->FindAnimation(L"Skill")->Reset();
}
