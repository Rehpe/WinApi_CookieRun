#include "pch.h"
#include "CPetIdleState.h"
#include "CObj.h"
#include "CFSM.h"
#include "CAnimation.h"
#include "CAnimator.h"
#include "CPet.h"
#include "CLevelManager.h"
#include "CLevel.h"
#include "CPlayer.h"

CPetIdleState::CPetIdleState()
{
}

CPetIdleState::~CPetIdleState()
{
}

void CPetIdleState::final_tick()
{
	//============IdleState에서 실행할 것
	
	CPet* pPet = dynamic_cast<CPet*>(GetOwnerFSM()->GetOwner());
	assert(pPet);

	//쿠키의 위치 가져오기
	CPlayer* pCookie = CLevelManager::GetInst()->GetCurLevel()->GetPlayer();
	
	//펫은 플레이어의 뒷쪽에 항상 붙어있는다.
	Vec2 vPos = pCookie->GetPos() - pPet->GetDefaultOffset();
	pPet->SetPos(vPos);

	//=============IdleState의 전환 조건

	//자석 아이템 획득 시
	if (pPet->IsMagnet())
	{
		GetOwnerFSM()->ChangeState(L"Magnet");
		return;
	}

	//보너스 타임 진입 시
	if (pCookie->IsBonus())
	{
		if (pCookie->IsOnSummon())
			return;

		GetOwnerFSM()->ChangeState(L"BonusEnter");
		return;
	}

	//스킬 사용시
	if (pPet->IsOnSkill())
	{
		GetOwnerFSM()->ChangeState(L"Skill");
		return;
	}

}


void CPetIdleState::Enter()
{
	//어떤 펫인지 알아낸다.
	CPet* pPet = dynamic_cast<CPet*>(GetOwnerFSM()->GetOwner());

	//펫이 아니라면 에러처리
	assert(pPet);

	//Idle 애니메이션을 재생한다
	GetOwnerFSM()->GetOwner()->GetAnimator()->Play(L"Idle", true);
}

void CPetIdleState::Exit()
{
	//해당 State 애니메이션을 Reset시킨다
	GetOwnerFSM()->GetOwner()->GetAnimator()->FindAnimation(L"Idle")->Reset();
}
