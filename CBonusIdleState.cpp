#include "pch.h"
#include "CBonusIdleState.h"
#include "CObj.h"
#include "CKeyManager.h"
#include "CTimeManager.h"
#include "CAnimator.h"
#include "CPlayer.h"
#include "CAnimation.h"
#include "CRigidbody.h"
#include "CLevelManager.h"
#include "CBonusStage.h"

CBonusIdleState::CBonusIdleState()
{
}

CBonusIdleState::~CBonusIdleState()
{
}

void CBonusIdleState::final_tick()
{
	//어떤 쿠키인지 알아낸다.
	CPlayer* pCookie = dynamic_cast<CPlayer*>(GetOwnerFSM()->GetOwner());

	//상태 주인이 플레이어(쿠키)가 아니라면 에러처리
	assert(pCookie);

	//===========BonusIdleState에서 실행할 것
	

	//=============BonusIdleState의 전환 조건

	if (IsPressed(KEY::SPACE))
	{
		Vec2 V = pCookie->GetRigidbody()->GetVelocity();
		pCookie->GetRigidbody()->AddVelocity(Vec2(0.f, -(V.y + 300.f)));
		GetOwnerFSM()->ChangeState(L"BonusJump");
		return;
	}

}

void CBonusIdleState::Enter()
{
	//어떤 쿠키인지 알아낸다.
	CPlayer* pCookie = dynamic_cast<CPlayer*>(GetOwnerFSM()->GetOwner());

	//BonusIdle 애니메이션을 재생한다
	pCookie->GetAnimator()->Play(L"BonusIdle", true);
}

void CBonusIdleState::Exit()
{
	//해당 State 애니메이션을 Reset시킨다
	GetOwnerFSM()->GetOwner()->GetAnimator()->FindAnimation(L"BonusIdle")->Reset();
}
