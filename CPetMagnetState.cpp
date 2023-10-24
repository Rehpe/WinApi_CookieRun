#include "pch.h"
#include "CPetMagnetState.h"
#include "CObj.h"
#include "CFSM.h"
#include "CAnimation.h"
#include "CAnimator.h"
#include "CPet.h"
#include "CLevelManager.h"
#include "CLevel.h"
#include "CPlayer.h"
#include "CSound.h"
#include "CResourceManager.h"

CPetMagnetState::CPetMagnetState()
{
}

CPetMagnetState::~CPetMagnetState()
{
}

void CPetMagnetState::final_tick()
{
	//============MagnetState에서 실행할 것

	CPet* pPet = dynamic_cast<CPet*>(GetOwnerFSM()->GetOwner());
	assert(pPet);

	//쿠키의 위치 가져오기
	CPlayer* pCookie = CLevelManager::GetInst()->GetCurLevel()->GetPlayer();

	//펫은 플레이어의 뒷쪽에 항상 붙어있는다.
	//Vec2 vPos = pCookie->GetPos() - Vec2(150.f, 100.f);
	//pPet->SetPos(vPos);

	//=============IdleState의 전환 조건

	//자석 아이템 지속시간 종료 시
	if (!pPet->IsMagnet())
	{
		GetOwnerFSM()->ChangeState(L"Idle");
		return;
	}

	//보너스 타임 진입 시
	if (pCookie->IsBonus())
	{
		if (pCookie->IsOnSummon())
			return;

		GetOwnerFSM()->ChangeState(L"BonusEnter");
		return;
	}

	
}

void CPetMagnetState::Enter()
{
	//어떤 펫인지 알아낸다.
	CPet* pPet = dynamic_cast<CPet*>(GetOwnerFSM()->GetOwner());

	//펫이 아니라면 에러처리
	assert(pPet);

	//Magnet 애니메이션을 재생한다
	GetOwnerFSM()->GetOwner()->GetAnimator()->Play(L"Magnet", true);

	//Magnet 효과음을 재생한다
	m_pMagnetSFX = CResourceManager::GetInst()->LoadSound(L"SFX_Magnet", L"sound\\General\\SFX_Magnet.wav");
	m_pMagnetSFX->Play(true);
}

void CPetMagnetState::Exit()
{
	//해당 State 애니메이션을 Reset시킨다
	GetOwnerFSM()->GetOwner()->GetAnimator()->FindAnimation(L"Magnet")->Reset();

	//효과음 off
	m_pMagnetSFX->Stop();
}
