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
	//============SkillState에서 실행할 것

	CPetMilkBottle* pPet = dynamic_cast<CPetMilkBottle*>(GetOwnerFSM()->GetOwner());
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

	//벡터 안의 모든 젤리를 생성했다면 스킬 사용중을 해제한다.
	if (m_vecMilkJelly.empty())
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

void CMilkBottleSkillState::Enter()
{
	//어떤 펫인지 알아낸다.
	CPet* pPet = dynamic_cast<CPet*>(GetOwnerFSM()->GetOwner());

	//펫이 아니라면 에러처리
	assert(pPet);

	//Skill 애니메이션을 재생한다
	pPet->GetAnimator()->Play(L"Skill", true);

	//생성할 우유젤리 10개를 벡터에 담아놓는다
	for (int i = 0; i < 10; i++)
	{
		CJelly* pMilkJelly = new CJelly;
		pMilkJelly->init(ITEM_TYPE::PETJELLY_MILK);
		m_vecMilkJelly.push_back(pMilkJelly);
	}
}


void CMilkBottleSkillState::Exit()
{
	//해당 State 애니메이션을 Reset시킨다
	GetOwnerFSM()->GetOwner()->GetAnimator()->FindAnimation(L"Skill")->Reset();
}
