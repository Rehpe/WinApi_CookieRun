#include "pch.h"
#include "CRunState.h"
#include "CObj.h"
#include "CKeyManager.h"
#include "CTimeManager.h"
#include "CAnimator.h"
#include "CPlayer.h"
#include "CAnimation.h"
#include "CRigidbody.h"
#include "CMilkCookie.h"
#include "CSnowSugarCookie.h"

CRunState::CRunState()
{
}

CRunState::~CRunState()
{
}

void CRunState::final_tick()
{
	//어떤 쿠키인지 알아낸다.
	CPlayer* pCookie = dynamic_cast<CPlayer*>(GetOwnerFSM()->GetOwner());
	
	//상태 주인이 플레이어(쿠키)가 아니라면 에러처리
	assert(pCookie);

	
	//============RunState에서 실행할 것

	//소유 쿠키의 위치 가져오기
	Vec2 vPos = pCookie->GetPos();

	//GetRigidbody()->AddForce(Vec2(300.f, 0.f)) ;

	if (IsPressed(KEY::LEFT))
	{
		vPos += Vec2(-700.f, 0.f) * DT;
	}
	if (IsPressed(KEY::RIGHT))
	{
		vPos += Vec2(700.f, 0.f) * DT;
	}

	pCookie->SetPos(vPos);


	//=============RunState의 전환 조건

	if (IsTap(KEY::SPACE))
	{
		//어떤 쿠키인지 알아낸다.
		CPlayer* pCookie = dynamic_cast<CPlayer*>(GetOwnerFSM()->GetOwner());

		pCookie->GetRigidbody()->AddVelocity(Vec2(0.f, -600.f));
		pCookie->GetRigidbody()->SetGround(false);
		GetOwnerFSM()->ChangeState(L"Jump");
		return;
	}

	if (IsPressed(KEY::DOWN))
	{
		GetOwnerFSM()->ChangeState(L"Slide");
		return;
	}

	if (pCookie->IsCrashed())
	{
		GetOwnerFSM()->ChangeState(L"Crash");
		return;
	}

	if (pCookie->GetHP() <= 0.f)
	{
		GetOwnerFSM()->ChangeState(L"Dead");
	}
	
	//부스터를 먹었다면
	
	//보너스타임 조건을 만족했다면
	if (pCookie->IsBonus())
	{
		if (pCookie->IsOnSummon())
			return;

		GetOwnerFSM()->ChangeState(L"BonusEnter");
		return;
	}

	//스킬 사용
	if (pCookie->IsOnSkill())
	{
		//우유맛 쿠키라면
		pCookie = dynamic_cast<CMilkCookie*>(pCookie);
		if (pCookie != nullptr)
		{
			GetOwnerFSM()->ChangeState(L"Skill");
			return;
		}
		pCookie = dynamic_cast<CSnowSugarCookie*>(pCookie);
		if (pCookie != nullptr)
		{
			GetOwnerFSM()->ChangeState(L"SkillEnter");
			return;
		}

		else
		{
			return;
		}
	}
}

void CRunState::Enter()
{
	//어떤 쿠키인지 알아낸다.
	CPlayer* pCookie = dynamic_cast<CPlayer*>(GetOwnerFSM()->GetOwner());

	pCookie->SetJump(false);

	//쿠키의 점프 카운트 2로 복구
	int iJumpCount = pCookie->GetJumpCount();
	pCookie->SetJumpCount(2);

	if (pCookie->IsBooster())
	{//Booster상태면 Booster애니메이션을 재생한다
		GetOwnerFSM()->GetOwner()->GetAnimator()->Play(L"Boost", true);
		return;
	}
	//Run 애니메이션을 재생한다
	GetOwnerFSM()->GetOwner()->GetAnimator()->Play(L"Run", true);


}

void CRunState::Exit()
{
	//해당 State 애니메이션을 Reset시킨다
	GetOwnerFSM()->GetOwner()->GetAnimator()->FindAnimation(L"Run")->Reset();
}
