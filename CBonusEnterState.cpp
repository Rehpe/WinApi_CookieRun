#include "pch.h"
#include "CBonusEnterState.h"
#include "CObj.h"
#include "CKeyManager.h"
#include "CTimeManager.h"
#include "CAnimator.h"
#include "CPlayer.h"
#include "CAnimation.h"
#include "CRigidbody.h"
#include "CLevelManager.h"
#include "CBonusStage.h"
#include "CLevel.h"
#include "CPet.h"
#include "CCamera.h"
#include "CUI.h"
#include "CStage1.h"

CBonusEnterState::CBonusEnterState()
{
}

CBonusEnterState::~CBonusEnterState()
{
}

void CBonusEnterState::final_tick()
{
	//어떤 쿠키인지 알아낸다.
	CPlayer* pCookie = dynamic_cast<CPlayer*>(GetOwnerFSM()->GetOwner());

	//상태 주인이 플레이어(쿠키)가 아니라면 에러처리
	assert(pCookie);

	//===========CBonusEnterState에서 실행할 것

	
	
	//-y방향으로 상승
	Vec2 vPos = pCookie->GetPos();
	vPos.y -= 2.f;
	if (vPos.y <= 0)
		vPos.y = 0;

	pCookie->SetPos(Vec2(vPos.x, vPos.y));

	
	//=============CBonusEnterState의 전환 조건
	
	//무조건 Bonus Enter Animation이 끝나야만 전환 가능
	if (GetOwnerFSM()->GetOwner()->GetAnimator()->FindAnimation(L"BonusEnter")->IsFinish())
	{
		//현재 레벨이 Bonus 레벨로 바뀌었는지 체크
		CBonusStage* pBonusLevel = dynamic_cast<CBonusStage*>(CLevelManager::GetInst()->GetCurLevel());
		if (pBonusLevel != nullptr)
		{
			GetOwnerFSM()->ChangeState(L"BonusIdle");
		}
	}
}

void CBonusEnterState::Enter()
{
	//어떤 쿠키인지 알아낸다.
	CPlayer* pCookie = dynamic_cast<CPlayer*>(GetOwnerFSM()->GetOwner());

	//땅 off
	pCookie->GetRigidbody()->SetGround(false);
	
	//쿠키의 중력 잠시 off
	pCookie->GetRigidbody()->SetGravityAccel(0.f);

	//현재 진입점 기억하기
	pCookie->SavePos();

	//쿠키, 펫, UI, PlayerSkill Layer 복사해서 새로운 레벨에 넣기?
	CPet* pPet = CLevelManager::GetInst()->GetCurLevel()->GetPet();


	vector<CObj*> vecUI = CLevelManager::GetInst()->GetCurLevel()->GetLayer(LAYER::UI);
	vector<CObj*> vecPlayerSkill = CLevelManager::GetInst()->GetCurLevel()->GetLayer(LAYER::PLAYER_SKILL);

	CLevel* pLevel = CLevelManager::GetInst()->GetLevel((int)LEVEL_TYPE::BONUS);
	
	pLevel->AddObject(pCookie, LAYER::PLAYER);
	pLevel->AddObject(pPet, LAYER::PET);

	for (int i = 0; i < vecUI.size(); i++)
	{
		CUI* pUI = static_cast<CUI*>(vecUI[i]);

		pLevel->AddObject(pUI, LAYER::UI);
	}

	for (int i = 0; i < vecPlayerSkill.size(); i++)
	{
		CObj* pPlayerSkill = static_cast<CUI*>(vecPlayerSkill[i]);

		pLevel->AddObject(pPlayerSkill, LAYER::PLAYER_SKILL);
	}

	static_cast<CBonusStage*>(pLevel)->SetScoreText(static_cast<CStage1*>(CLevelManager::GetInst()->GetCurLevel())->GetScoreText());
	static_cast<CBonusStage*>(pLevel)->SetCoinText(static_cast<CStage1*>(CLevelManager::GetInst()->GetCurLevel())->GetCoinText());

	ChangeLevel(LEVEL_TYPE::BONUS);


	//카메라 이펙트 -> fade in/out
	CCamera::GetInst()->FadeIn(0.8f);

	//BonusEnter 애니메이션을 재생한다
	pCookie->GetAnimator()->Play(L"BonusEnter", false);
}

void CBonusEnterState::Exit()
{
	//쿠키의 중력 다시 원상복구
	GetOwnerFSM()->GetOwner()->GetRigidbody()->SetGravityAccel(500.f);

	//해당 State 애니메이션을 Reset시킨다
	GetOwnerFSM()->GetOwner()->GetAnimator()->FindAnimation(L"BonusEnter")->Reset();
}
