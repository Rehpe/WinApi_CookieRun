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

	//펫이 타켓 포인트로 나간다
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

	//약 0.5초 뒤에 얼음곰파티 생명물약 아이템을 생성하고 돌아온다.
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
	//=============SkillState의 전환 조건

	//스킬 사용 상태가 아니라면 Idle로 전환
	if (!pPet->IsOnSkill())
	{
		GetOwnerFSM()->ChangeState(L"Idle");
		return;
	}
}

void CSnowGlobeSkillState::Enter()
{
	//어떤 펫인지 알아낸다.
	CPet* pPet = dynamic_cast<CPet*>(GetOwnerFSM()->GetOwner());

	//펫이 아니라면 에러처리
	assert(pPet);

	//Skill 애니메이션을 재생한다
	pPet->GetAnimator()->Play(L"Skill", true);
}

void CSnowGlobeSkillState::Exit()
{
	//해당 State 애니메이션을 Reset시킨다
	GetOwnerFSM()->GetOwner()->GetAnimator()->FindAnimation(L"Skill")->Reset();
}
