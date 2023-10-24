#include "pch.h"
#include "CPetBonusJumpState.h"
#include "CObj.h"
#include "CFSM.h"
#include "CAnimation.h"
#include "CAnimator.h"
#include "CPet.h"
#include "CLevelManager.h"
#include "CLevel.h"
#include "CPlayer.h"
#include "CKeyManager.h"

CPetBonusJumpState::CPetBonusJumpState()
{
}

CPetBonusJumpState::~CPetBonusJumpState()
{
}

void CPetBonusJumpState::final_tick()
{
	//============BonusJumpState에서 실행할 것

	CPet* pPet = dynamic_cast<CPet*>(GetOwnerFSM()->GetOwner());
	assert(pPet);

	//쿠키의 위치 가져오기
	CPlayer* pCookie = pPet->GetPlayer();

	//펫은 보너스 스테이지 진입 시 플레이어의 윗쪽에 항상 붙어있는다.
	Vec2 vPos = pCookie->GetPos() - Vec2(0.f, 100.f);
	if (pCookie->IsBigger())
		vPos = pCookie->GetPos() - Vec2(0.f, 180.f);
	pPet->SetPos(vPos);

	//=============BonusJumpState의 전환 조건

	if (IsRelease(KEY::SPACE))
	{
		GetOwnerFSM()->ChangeState(L"BonusIdle");
		return;
	}

	if (!pCookie->IsBonus())
	{
		GetOwnerFSM()->ChangeState(L"BonusExit");
		return;
	}
}

void CPetBonusJumpState::Enter()
{
	//어떤 펫인지 알아낸다.
	CPet* pPet = dynamic_cast<CPet*>(GetOwnerFSM()->GetOwner());

	//펫이 아니라면 에러처리
	assert(pPet);

	//BonusJump 애니메이션을 재생한다
	GetOwnerFSM()->GetOwner()->GetAnimator()->Play(L"BonusJump", true);
}

void CPetBonusJumpState::Exit()
{
	//해당 State 애니메이션을 Reset시킨다
	GetOwnerFSM()->GetOwner()->GetAnimator()->FindAnimation(L"BonusJump")->Reset();
}
