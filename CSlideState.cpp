#include "pch.h"
#include "CSlideState.h"
#include "CObj.h"
#include "CKeyManager.h"
#include "CTimeManager.h"
#include "CAnimator.h"
#include "CPlayer.h"
#include "CAnimation.h"
#include "CCollider.h"
#include "CMilkCookie.h"
#include "CGingerBraveCookie.h"
#include "CSound.h"

CSlideState::CSlideState()
{
}

CSlideState::~CSlideState()
{
}

void CSlideState::final_tick()
{
	//어떤 쿠키인지 알아낸다.
	CPlayer* pCookie = dynamic_cast<CPlayer*>(GetOwnerFSM()->GetOwner());

	//상태 주인이 플레이어(쿠키)가 아니라면 에러처리
	assert(pCookie);


	//============SlideState에서 실행할 것

	//소유 쿠키의 충돌체를 납작하게 조정한다
	CCollider* pCollider = pCookie->GetCollider();
	pCollider->SetOffsetScale(Vec2(0.f, -60.f)); //충돌체 크기 100, 65
	pCollider->SetOffsetPos(Vec2(0.f, 30.f));

	//부스터 상태인 경우 충돌체 오프셋을 약간 수정한다
	if (pCookie->IsBigger() == true)
	{
		pCollider->SetOffsetScale(Vec2(0.f,-120.f));
		pCollider->SetOffsetPos(Vec2(0.f, 60.f));
	}
	
	//=============RunState의 전환 조건

	//슬라이드키를 떼면 Run으로 복귀
	if (IsRelease(KEY::DOWN))
	{
		GetOwnerFSM()->ChangeState(L"Run");
		return;
	}

	if (IsTap(KEY::SPACE))
	{
		GetOwnerFSM()->ChangeState(L"Jump");
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

		else
		{
			return;
		}
	}
	//부스터를 먹었다면
	//보너스타임 조건을 만족했다면
}

void CSlideState::Enter()
{
	//어떤 쿠키인지 알아낸다.
	CPlayer* pCookie = dynamic_cast<CPlayer*>(GetOwnerFSM()->GetOwner());

	pCookie->SetJump(false);

	//Slide 애니메이션을 재생한다
	pCookie->GetAnimator()->Play(L"Slide", true);

	//Slide 효과음을 재생한다.
	pCookie->GetSlideSFX()->Play();

	//소유 쿠키의 충돌체를 납작하게 조정한다
	CCollider* pCollider = pCookie->GetCollider();
	//pCollider->SetOffsetScale(Vec2(0.f, -30.f)); //충돌체 크기 100, 65
	//pCollider->SetOffsetPos(Vec2(0.f, -30.f));
}

void CSlideState::Exit()
{
	//어떤 쿠키인지 알아낸다.
	CPlayer* pCookie = dynamic_cast<CPlayer*>(GetOwnerFSM()->GetOwner());

	//해당 State 애니메이션을 Reset시킨다
	pCookie->GetAnimator()->FindAnimation(L"Slide")->Reset();

	//줄어든 충돌체 크기를 원상복구시킨다
	pCookie->GetCollider()->SetOffsetScale(Vec2(0.f, 0.f));	//충돌체 크기 100, 125
	pCookie->GetCollider()->SetOffsetPos(Vec2(0.f, 0.f));

}
