#include "pch.h"
#include "CJumpState.h"
#include "CObj.h"
#include "CKeyManager.h"
#include "CTimeManager.h"
#include "CAnimator.h"
#include "CPlayer.h"
#include "CAnimation.h"
#include "CRigidbody.h"
#include "CCamera.h"
#include "CSound.h"
#include "CSnowSugarCookie.h"

CJumpState::CJumpState()
{
}

CJumpState::~CJumpState()
{
}

void CJumpState::final_tick()
{
	//어떤 쿠키인지 알아낸다.
	CPlayer* pCookie = dynamic_cast<CPlayer*>(GetOwnerFSM()->GetOwner());

	//상태 주인이 플레이어(쿠키)가 아니라면 에러처리
	assert(pCookie);

	//===========RunState에서 실행할 것

	


	//=============JumpState의 전환 조건

	//점프 버튼을 눌렀고 && 쿠키가 현재 점프중이고 && 쿠키의 남은 점프횟수가 0보다 크다면
	if (IsTap(KEY::SPACE) && pCookie->IsJump() && pCookie->GetJumpCount() > 0)
	{
		Vec2 V = pCookie->GetRigidbody()->GetVelocity();
		pCookie->GetRigidbody()->AddVelocity(Vec2(0.f, -(V.y + 600.f)));
		GetOwnerFSM()->ChangeState(L"DoubleJump");
		return;
	}

	//쿠키가 땅에 닿은 상태면
	if (pCookie->GetRigidbody()->IsGround())
	{
		GetOwnerFSM()->ChangeState(L"Run");
		if (pCookie->GetName() == L"SnowSugarCookie" && pCookie->IsOnSkill())
		{
			GetOwnerFSM()->ChangeState(L"Skill");
		}

		//거대화 상태에서 Jump->Run 상태로 돌아오면 카메라 흔들림 + 쿵쿵 효과음 재생
		if (pCookie->IsBigger() || (pCookie->GetName() == L"SnowSugarCookie" && pCookie->IsOnSkill()))
		{
			CCamera::GetInst()->CameraShake(5.f, 300.f, 0.3f);
			pCookie->GetBiggerLandSFX()->Play();
		}
		return;
	}

	if (pCookie->IsCrashed())
	{
		GetOwnerFSM()->ChangeState(L"Crash");
		return;
	}
	
	//보너스타임 조건을 만족했다면
	if (pCookie->IsBonus())
	{
		if (pCookie->IsOnSummon())
			return;

		GetOwnerFSM()->ChangeState(L"BonusEnter");
		return;
	}
}

void CJumpState::Enter()
{
	//어떤 쿠키인지 알아낸다.
	CPlayer* pCookie = dynamic_cast<CPlayer*>(GetOwnerFSM()->GetOwner());

	pCookie->SetJump(true);

	//Jump 애니메이션을 재생한다
	pCookie->GetAnimator()->Play(L"Jump", false);
	//스킬 사용중인 눈설탕맛 쿠키라면 전용 애니메이션 출력
	if (pCookie->GetName() == L"SnowSugarCookie" && pCookie->IsOnSkill())
	{
		pCookie->GetAnimator()->Play(L"SkillJump", false);
	}

	//현재 상태에 따라 다른 Jump 효과음을 재생한다.
	if(pCookie->IsBigger() || (pCookie->GetName() == L"SnowSugarCookie" && pCookie->IsOnSkill()))
		pCookie->GetBiggerJumpSFX()->Play();
	else
		pCookie->GetJumpSFX()->Play();

	//쿠키의 점프 카운트 감소
	int iJumpCount = pCookie->GetJumpCount();
	pCookie->SetJumpCount(--iJumpCount);
}

void CJumpState::Exit()
{
	//어떤 쿠키인지 알아낸다.
	CPlayer* pCookie = dynamic_cast<CPlayer*>(GetOwnerFSM()->GetOwner());

	//해당 State 애니메이션을 Reset시킨다
	GetOwnerFSM()->GetOwner()->GetAnimator()->FindAnimation(L"Jump")->Reset();
	//스킬 사용중인 눈설탕맛 쿠키라면 전용 애니메이션 Reset
	if (pCookie->GetName() == L"SnowSugarCookie" && pCookie->IsOnSkill())
	{
		pCookie->GetAnimator()->FindAnimation(L"SkillJump")->Reset();
	}
}
