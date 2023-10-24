#include "pch.h"
#include "CPetBonusIdleState.h"
#include "CObj.h"
#include "CFSM.h"
#include "CAnimation.h"
#include "CAnimator.h"
#include "CPet.h"
#include "CLevelManager.h"
#include "CLevel.h"
#include "CPlayer.h"
#include "CKeyManager.h"

CPetBonusIdleState::CPetBonusIdleState()
{
}

CPetBonusIdleState::~CPetBonusIdleState()
{
}

void CPetBonusIdleState::final_tick()
{
	//============BonusIdleState에서 실행할 것

	CPet* pPet = dynamic_cast<CPet*>(GetOwnerFSM()->GetOwner());
	assert(pPet);

	//쿠키의 위치 가져오기
	//CPlayer* pCookie = CLevelManager::GetInst()->GetCurLevel()->GetPlayer();
	CPlayer* pCookie = pPet->GetPlayer();

	//펫은 보너스 스테이지 진입 시 플레이어의 윗쪽에 항상 붙어있는다.
	Vec2 vPos = pCookie->GetPos() - Vec2(0.f, 100.f);

	if(pCookie->IsBigger())
		vPos = pCookie->GetPos() - Vec2(0.f, 180.f);

	pPet->SetPos(vPos);

	//=============BonusIdleState의 전환 조건

	if (IsTap(KEY::SPACE))
	{
		GetOwnerFSM()->ChangeState(L"BonusJump");
		return;
	}
	if (!pCookie->IsBonus())
	{
		GetOwnerFSM()->ChangeState(L"BonusExit");
		return;
	}
}

void CPetBonusIdleState::Enter()
{
	//어떤 펫인지 알아낸다.
	CPet* pPet = dynamic_cast<CPet*>(GetOwnerFSM()->GetOwner());

	//펫이 아니라면 에러처리
	assert(pPet);

	//BonusIdle 애니메이션을 재생한다
	GetOwnerFSM()->GetOwner()->GetAnimator()->Play(L"BonusIdle", true);
}

void CPetBonusIdleState::Exit()
{
	//해당 State 애니메이션을 Reset시킨다
	GetOwnerFSM()->GetOwner()->GetAnimator()->FindAnimation(L"BonusIdle")->Reset();
}
