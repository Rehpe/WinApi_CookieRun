#include "pch.h"
#include "CPetBonusExitState.h"
#include "CObj.h"
#include "CFSM.h"
#include "CAnimation.h"
#include "CAnimator.h"
#include "CPet.h"
#include "CLevelManager.h"
#include "CLevel.h"
#include "CPlayer.h"

CPetBonusExitState::CPetBonusExitState()
{
}

CPetBonusExitState::~CPetBonusExitState()
{
}

void CPetBonusExitState::final_tick()
{
	//============BonusExitState에서 실행할 것

	CPet* pPet = dynamic_cast<CPet*>(GetOwnerFSM()->GetOwner());
	assert(pPet);

	//쿠키의 위치 가져오기
	CPlayer* pCookie = CLevelManager::GetInst()->GetCurLevel()->GetPlayer();

	//펫은 보너스 스테이지 진입 시 플레이어의 윗쪽에 항상 붙어있는다.
	Vec2 vPos = pCookie->GetPos() - Vec2(0.f, 130.f);



	pPet->SetPos(vPos);

	//=============BonusExitState의 전환 조건

	//무조건 BonusExit Animation이 끝나야만 전환 가능
	if (GetOwnerFSM()->GetOwner()->GetAnimator()->FindAnimation(L"BonusExit")->IsFinish())
	{
		GetOwnerFSM()->ChangeState(L"Idle");
		return;
	}
}

void CPetBonusExitState::Enter()
{
	//어떤 펫인지 알아낸다.
	CPet* pPet = dynamic_cast<CPet*>(GetOwnerFSM()->GetOwner());

	//펫이 아니라면 에러처리
	assert(pPet);

	//BonusExit 애니메이션을 재생한다
	GetOwnerFSM()->GetOwner()->GetAnimator()->Play(L"BonusExit", false);
}

void CPetBonusExitState::Exit()
{
	//해당 State 애니메이션을 Reset시킨다
	GetOwnerFSM()->GetOwner()->GetAnimator()->FindAnimation(L"BonusExit")->Reset();
}
