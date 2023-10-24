#include "pch.h"
#include "CBonusJumpState.h"
#include "CObj.h"
#include "CKeyManager.h"
#include "CTimeManager.h"
#include "CAnimator.h"
#include "CPlayer.h"
#include "CAnimation.h"
#include "CRigidbody.h"
#include "CLevelManager.h"
#include "CBonusStage.h"

CBonusJumpState::CBonusJumpState()
{
}

CBonusJumpState::~CBonusJumpState()
{
}

void CBonusJumpState::final_tick()
{
	//어떤 쿠키인지 알아낸다.
	CPlayer* pCookie = dynamic_cast<CPlayer*>(GetOwnerFSM()->GetOwner());

	//상태 주인이 플레이어(쿠키)가 아니라면 에러처리
	assert(pCookie);

	//===========BonusJumpState에서 실행할 것


	//=============BonusJumpState의 전환 조건

	if (IsRelease(KEY::SPACE))
	{
		GetOwnerFSM()->ChangeState(L"BonusIdle");
		return;
	}
	//if (!pCookie->IsBonus())
	//{
	//	GetOwnerFSM()->ChangeState(L"BonusExit");
	//	return;
	//}
}

void CBonusJumpState::Enter()
{
	//어떤 쿠키인지 알아낸다.
	CPlayer* pCookie = dynamic_cast<CPlayer*>(GetOwnerFSM()->GetOwner());

	//BonusJump 애니메이션을 재생한다
	pCookie->GetAnimator()->Play(L"BonusJump", true);
}

void CBonusJumpState::Exit()
{
	//해당 State 애니메이션을 Reset시킨다
	GetOwnerFSM()->GetOwner()->GetAnimator()->FindAnimation(L"BonusJump")->Reset();
}
