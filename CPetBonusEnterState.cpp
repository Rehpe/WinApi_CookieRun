#include "pch.h"
#include "CPetBonusEnterState.h"
#include "CObj.h"
#include "CFSM.h"
#include "CAnimation.h"
#include "CAnimator.h"
#include "CPet.h"
#include "CLevelManager.h"
#include "CLevel.h"
#include "CPlayer.h"

CPetBonusEnterState::CPetBonusEnterState()
{
}

CPetBonusEnterState::~CPetBonusEnterState()
{
}

void CPetBonusEnterState::final_tick()
{
	//============BonusEnterState에서 실행할 것

	CPet* pPet = dynamic_cast<CPet*>(GetOwnerFSM()->GetOwner());
	assert(pPet);

	////쿠키의 위치 가져오기
	//CPlayer* pCookie = CLevelManager::GetInst()->GetCurLevel()->GetPlayer();

	//펫은 보너스 스테이지 진입 시 플레이어의 윗쪽에 항상 붙어있는다.
	Vec2 vPos = pPet->GetPlayer()->GetPos() - Vec2(0.f, 100.f);
	pPet->SetPos(vPos);

	//=============BonusEnterState의 전환 조건

	//무조건 BonusEnter Animation이 끝나야만 전환 가능
	if (GetOwnerFSM()->GetOwner()->GetAnimator()->FindAnimation(L"BonusEnter")->IsFinish())
	{
		GetOwnerFSM()->ChangeState(L"BonusIdle");
		return;
	}
}

void CPetBonusEnterState::Enter()
{
	//어떤 펫인지 알아낸다.
	CPet* pPet = dynamic_cast<CPet*>(GetOwnerFSM()->GetOwner());

	//펫이 아니라면 에러처리
	assert(pPet);

	//BonusEnter 애니메이션을 재생한다
	GetOwnerFSM()->GetOwner()->GetAnimator()->Play(L"BonusEnter", false);
}

void CPetBonusEnterState::Exit()
{
	//해당 State 애니메이션을 Reset시킨다
	GetOwnerFSM()->GetOwner()->GetAnimator()->FindAnimation(L"BonusEnter")->Reset();
}
