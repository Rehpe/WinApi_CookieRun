#include "pch.h"
#include "CDeadState.h"
#include "CObj.h"
#include "CKeyManager.h"
#include "CTimeManager.h"
#include "CAnimator.h"
#include "CPlayer.h"
#include "CAnimation.h"
#include "CRigidbody.h"
#include "CCamera.h"
#include "CLevelManager.h"
#include "CLevel.h"
#include "CResultLevel.h"
#include "CSound.h"

CDeadState::CDeadState()
{
}

CDeadState::~CDeadState()
{
}

void CDeadState::final_tick()
{
	//어떤 쿠키인지 알아낸다.
	CPlayer* pCookie = dynamic_cast<CPlayer*>(GetOwnerFSM()->GetOwner());

	//상태 주인이 플레이어(쿠키)가 아니라면 에러처리
	assert(pCookie);

	//===========DeadState에서 실행할 것
	pCookie->SetHP(0.f);

	//=============DeadState의 전환 조건

	//무조건 Crash Animation이 끝나야만 전환 가능
	if (GetOwnerFSM()->GetOwner()->GetAnimator()->FindAnimation(L"Dead")->IsFinish())
	{
		pCookie->SetSpeed(0.f);
		static float fAccTime = 0;
		fAccTime += DT;

		//1초 후에
		if (fAccTime >= 1)
		{
			//부활 기능이 있다면 부활

			//없다면 게임 결과창으로

			fAccTime = 0;

			pCookie->SetPlayerDead(true);
			pCookie->SetSpeed(0.f);
			pCookie->GetGameOverSFX()->Play();
			ChangeLevel(LEVEL_TYPE::RESULT);
		}
	}
}

void CDeadState::Enter()
{
	//어떤 쿠키인지 알아낸다.
	CPlayer* pCookie = dynamic_cast<CPlayer*>(GetOwnerFSM()->GetOwner());

	//Dead 애니메이션을 재생한다
	pCookie->GetAnimator()->Play(L"Dead", false);
}

void CDeadState::Exit()
{
	//해당 State 애니메이션을 Reset시킨다
	GetOwnerFSM()->GetOwner()->GetAnimator()->FindAnimation(L"Dead")->Reset();
}
