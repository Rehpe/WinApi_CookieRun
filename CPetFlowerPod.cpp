#include "pch.h"
#include "CPetFlowerPod.h"
#include "CResourceManager.h"
#include "CFSM.h"
#include "CAnimator.h"
#include "CPetIdleState.h"
#include "CPetMagnetState.h"
#include "CPetSkillState.h"
#include "CPetBonusEnterState.h"
#include "CPetBonusIdleState.h"
#include "CPetBonusJumpState.h"
#include "CPetBonusExitState.h"
#include "CFlowerPodSkillState.h"

CPetFlowerPod::CPetFlowerPod()
	: m_iFlowerJellyScore(10000)
{
	//아틀라스 텍스처 Load
	SetPetTex(CResourceManager::GetInst()->LoadTexture(L"MilkBottle_Atlas", L"texture\\pet\\MilkBottle_Atlas.png"));

	//애니메이션 Load 
	GetAnimator()->LoadAnimation(L"animation\\Pet\\FlowerPod\\FlowerPod_Idle.anim");
	GetAnimator()->LoadAnimation(L"animation\\Pet\\FlowerPod\\FlowerPod_Magnet.anim");
	GetAnimator()->LoadAnimation(L"animation\\Pet\\FlowerPod\\FlowerPod_Skill.anim");
	GetAnimator()->LoadAnimation(L"animation\\Pet\\FlowerPod\\FlowerPod_BonusEnter.anim");
	GetAnimator()->LoadAnimation(L"animation\\Pet\\FlowerPod\\FlowerPod_BonusIdle.anim");
	GetAnimator()->LoadAnimation(L"animation\\Pet\\FlowerPod\\FlowerPod_BonusJump.anim");
	GetAnimator()->LoadAnimation(L"animation\\Pet\\FlowerPod\\FlowerPod_BonusExit.anim");

	GetFSM()->AddState(L"Idle", new CPetIdleState);
	GetFSM()->AddState(L"Magnet", new CPetMagnetState);
	GetFSM()->AddState(L"Skill", new CFlowerPodSkillState);
	GetFSM()->AddState(L"BonusEnter", new CPetBonusEnterState);
	GetFSM()->AddState(L"BonusIdle", new CPetBonusIdleState);
	GetFSM()->AddState(L"BonusJump", new CPetBonusJumpState);
	GetFSM()->AddState(L"BonusExit", new CPetBonusExitState);

	//처음 상태는 Idle
	GetFSM()->ChangeState(L"Idle");

	m_pMakeFlowerJellySFX = CResourceManager::GetInst()->LoadSound(L"Fairy_Pet_Make_Jelly", L"sound\\Cookie\\Fairy_Pet_Make_Jelly.wav");

	m_fSkillCoolTime = 15.f;
	SetScale(Vec2(70.f, 70.f));
}

CPetFlowerPod::~CPetFlowerPod()
{
}

void CPetFlowerPod::tick()
{
	CPet::tick();
}

void CPetFlowerPod::render(HDC _dc)
{
	CPet::render(_dc);
}
