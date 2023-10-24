#include "pch.h"
#include "CBonusExitState.h"
#include "CObj.h"
#include "CKeyManager.h"
#include "CTimeManager.h"
#include "CAnimator.h"
#include "CPlayer.h"
#include "CAnimation.h"
#include "CRigidbody.h"
#include "CLevelManager.h"
#include "CStage1.h"
#include "CEditorLevel.h"
#include "CCamera.h"
#include "CUI.h"

CBonusExitState::CBonusExitState()
{
}

CBonusExitState::~CBonusExitState()
{
}

void CBonusExitState::final_tick()
{
	//어떤 쿠키인지 알아낸다.
	CPlayer* pCookie = dynamic_cast<CPlayer*>(GetOwnerFSM()->GetOwner());

	//상태 주인이 플레이어(쿠키)가 아니라면 에러처리
	assert(pCookie);

	//===========CBonusExitState에서 실행할 것
	static float fDelay = 0;
	fDelay += DT;

	//=============CBonusExitState의 전환 조건

	//무조건 Bonus Exit Animation이 끝나야만 전환 가능
	if (fDelay >=1.5f)
	{
		GetOwnerFSM()->ChangeState(L"Run");
		fDelay = 0;
	}
}

void CBonusExitState::Enter()
{
	//어떤 쿠키인지 알아낸다.
	CPlayer* pCookie = dynamic_cast<CPlayer*>(GetOwnerFSM()->GetOwner());

	//쿠키의 중력 다시 원상복구
	pCookie->GetRigidbody()->SetGravityAccel(800.f);

	//BonusExit 애니메이션을 재생한다
	pCookie->GetAnimator()->Play(L"BonusExit", false);

}

void CBonusExitState::Exit()
{
	//어떤 쿠키인지 알아낸다.
	CPlayer* pCookie = dynamic_cast<CPlayer*>(GetOwnerFSM()->GetOwner());

	//해당 State 애니메이션을 Reset시킨다
	GetOwnerFSM()->GetOwner()->GetAnimator()->FindAnimation(L"BonusExit")->Reset();

}
