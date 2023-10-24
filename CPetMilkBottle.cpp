#include "pch.h"
#include "CPetMilkBottle.h"
#include "CResourceManager.h"
#include "CTexture.h"
#include "CAnimation.h"
#include "CAnimator.h"
#include "CFSM.h"
#include "CPetIdleState.h"
#include "CPetMagnetState.h"
#include "CPetSkillState.h"
#include "CPetBonusEnterState.h"
#include "CPetBonusIdleState.h"
#include "CPetBonusJumpState.h"
#include "CPetBonusExitState.h"
#include "CMilkBottleSkillState.h"
#include "CSound.h"

CPetMilkBottle::CPetMilkBottle()
{
	//��Ʋ�� �ؽ�ó Load
	SetPetTex(CResourceManager::GetInst()->LoadTexture(L"MilkBottle_Atlas", L"texture\\pet\\MilkBottle_Atlas.png"));

	//�ִϸ��̼� Load 
	GetAnimator()->LoadAnimation(L"animation\\Pet\\MilkBottle\\MilkBottle_Idle.anim");
	GetAnimator()->LoadAnimation(L"animation\\Pet\\MilkBottle\\MilkBottle_Magnet.anim");
	GetAnimator()->LoadAnimation(L"animation\\Pet\\MilkBottle\\MilkBottle_Skill.anim");
	GetAnimator()->LoadAnimation(L"animation\\Pet\\MilkBottle\\MilkBottle_BonusEnter.anim");
	GetAnimator()->LoadAnimation(L"animation\\Pet\\MilkBottle\\MilkBottle_BonusIdle.anim");
	GetAnimator()->LoadAnimation(L"animation\\Pet\\MilkBottle\\MilkBottle_BonusJump.anim");
	GetAnimator()->LoadAnimation(L"animation\\Pet\\MilkBottle\\MilkBottle_BonusExit.anim");

	GetFSM()->AddState(L"Idle", new CPetIdleState);
	GetFSM()->AddState(L"Magnet", new CPetMagnetState);
	GetFSM()->AddState(L"Skill", new CMilkBottleSkillState);
	GetFSM()->AddState(L"BonusEnter", new CPetBonusEnterState);
	GetFSM()->AddState(L"BonusIdle", new CPetBonusIdleState);
	GetFSM()->AddState(L"BonusJump", new CPetBonusJumpState);
	GetFSM()->AddState(L"BonusExit", new CPetBonusExitState);

	//ó�� ���´� Idle
	GetFSM()->ChangeState(L"Idle");

	m_pMakeMilkJellySFX = CResourceManager::GetInst()->LoadSound(L"Milk_Pet_Make_Jelly", L"sound\\Cookie\\Milk_Pet_Make_Jelly.wav");

	m_fSkillCoolTime = 15.f;
	SetScale(Vec2(70.f, 70.f));
}

CPetMilkBottle::~CPetMilkBottle()
{
}

void CPetMilkBottle::tick()
{
	CPet::tick();
}

void CPetMilkBottle::render(HDC _dc)
{
	CPet::render(_dc);
}
