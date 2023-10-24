#include "pch.h"
#include "CCrashState.h"
#include "CObj.h"
#include "CKeyManager.h"
#include "CTimeManager.h"
#include "CAnimator.h"
#include "CPlayer.h"
#include "CAnimation.h"
#include "CRigidbody.h"
#include "CCamera.h"
#include "CSound.h"

CCrashState::CCrashState()
{
}

CCrashState::~CCrashState()
{
}

void CCrashState::final_tick()
{
	//어떤 쿠키인지 알아낸다.
	CPlayer* pCookie = dynamic_cast<CPlayer*>(GetOwnerFSM()->GetOwner());

	//상태 주인이 플레이어(쿠키)가 아니라면 에러처리
	assert(pCookie);

	//===========CrashState에서 실행할 것
	pCookie->SetCrashed(true);



	//=============CrashState의 전환 조건

	//무조건 Crash Animation이 끝나야만 전환 가능
	if (GetOwnerFSM()->GetOwner()->GetAnimator()->FindAnimation(L"Crash")->IsFinish())
	{
		//쿠키가 부딪친 상태 && HP가 0보다 낮거나 같다면
		if (pCookie->IsCrashed() && pCookie->GetHP() <=0)
		{
			GetOwnerFSM()->ChangeState(L"CrashDead");
			return;
		}

		else if (pCookie->GetHP() > 0 && IsPressed(KEY::DOWN))
		{
			pCookie->SetCrashed(false);
			GetOwnerFSM()->ChangeState(L"Slide");
			return;
		}

		//위 상태가 아니라면 Run상태로 복귀
		else if (pCookie->GetHP() > 0)
		{
			pCookie->SetCrashed(false);
			GetOwnerFSM()->ChangeState(L"Run");
			return;
		}

		//부스터
		//보너스타임 조건을 만족했다면
		if (pCookie->IsBonus())
		{
			GetOwnerFSM()->ChangeState(L"BonusEnter");
			return;
		}
	}
}

void CCrashState::Enter()
{
	//어떤 쿠키인지 알아낸다.
	CPlayer* pCookie = dynamic_cast<CPlayer*>(GetOwnerFSM()->GetOwner());

	//쿠키 무적상태 변수를 true로 만든다.
	pCookie->SetInvincible(true);

	//카메라 shake를 준다
	CCamera::GetInst()->CameraShake(5.f, 300.f, 0.5f);

	//Crash 애니메이션을 재생한다
	pCookie->GetAnimator()->Play(L"Crash", false);

	//Crash 효과음을 재생한다
	pCookie->GetCrashSFX()->Play();
}

void CCrashState::Exit()
{
	//해당 State 애니메이션을 Reset시킨다
	GetOwnerFSM()->GetOwner()->GetAnimator()->FindAnimation(L"Crash")->Reset();
}
