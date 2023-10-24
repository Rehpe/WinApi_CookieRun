#include "pch.h"
#include "CSnowSugarSkillState.h"
#include "CSnowSugarCookie.h"
#include "CKeyManager.h"
#include "CRigidbody.h"
#include "CAnimator.h"
#include "CTimeManager.h"
#include "CAnimation.h"
#include "CSound.h"
#include "CCollider.h"
#include "CJelly.h"
#include <time.h>

CSnowSugarSkillState::CSnowSugarSkillState()
{
}

CSnowSugarSkillState::~CSnowSugarSkillState()
{
}

void CSnowSugarSkillState::final_tick()
{
	//어떤 쿠키인지 알아낸다.
	CSnowSugarCookie* pCookie = dynamic_cast<CSnowSugarCookie*>(GetOwnerFSM()->GetOwner());

	//상태 주인이 우유맛 쿠키가 아니라면 에러처리
	assert(pCookie);

	//============SnowSugarSkill에서 실행할 것
	pCookie->GetAnimator()->SetOffset(Vec2(0.f, 105.f));


	//랜덤시드 설정
	srand((unsigned)time(NULL));
	rand();

	//플레이어 앞 x축에서 랜덤한 y범위에 0.5초마다 눈꽃젤리를 생성한다.
	//생성한 눈꽃젤리가 장애물과 닿으면 장애물은 터진다


	//=============SnowSugarSkill의 전환 조건

	if (IsTap(KEY::SPACE))
	{
		//어떤 쿠키인지 알아낸다.
		CPlayer* pCookie = dynamic_cast<CPlayer*>(GetOwnerFSM()->GetOwner());

		pCookie->GetRigidbody()->AddVelocity(Vec2(0.f, -600.f));
		pCookie->GetRigidbody()->SetGround(false);
		GetOwnerFSM()->ChangeState(L"Jump");
		return;
	}

	if (!pCookie->IsOnSkill())
	{
		GetOwnerFSM()->ChangeState(L"SkillExit");
		return;
	}
}

void CSnowSugarSkillState::Enter()
{
	//어떤 쿠키인지 알아낸다.
	CSnowSugarCookie* pCookie = dynamic_cast<CSnowSugarCookie*>(GetOwnerFSM()->GetOwner());

	//SkillEnter 애니메이션을 재생한다
	pCookie->GetAnimator()->Play(L"SkillIdle", false);

	pCookie->SetScale(Vec2(360.f, 300.f));


	////SkillEnter 효과음을 재생한다.
	//pCookie->GetSkillEnterSFX()->Play();
}

void CSnowSugarSkillState::Exit()
{
	//어떤 쿠키인지 알아낸다.
	CSnowSugarCookie* pCookie = dynamic_cast<CSnowSugarCookie*>(GetOwnerFSM()->GetOwner());

	m_fDelay = 0;

	//해당 State에서 사용한 모든 애니메이션을 Reset시킨다
	pCookie->GetAnimator()->FindAnimation(L"SkillIdle")->Reset();

	////남은 몬스터가 있다면 Dead 애니메이션 재생 후 Dead시킴
	//vector<CObj*> vecMonsterLayer = CLevelManager::GetInst()->GetCurLevel()->GetLayer(LAYER::MONSTER);
	//for (int i = 0; i < vecMonsterLayer.size(); i++)
	//{
	//	CMilkMonster* pMon = static_cast<CMilkMonster*>(vecMonsterLayer[i]);
	//	pMon->GetAnimator()->Play(L"MonsterDead", false);
	//}
}
