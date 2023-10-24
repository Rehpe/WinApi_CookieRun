#include "pch.h"
#include "CPetChocoDrop.h"
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

CPetChocoDrop::CPetChocoDrop()
{
	//아틀라스 텍스처 Load
	SetPetTex(CResourceManager::GetInst()->LoadTexture(L"ChocoDrop_Atlas", L"texture\\pet\\ChocoDrop_Atlas.png"));

	//애니메이션 Load 
	GetAnimator()->LoadAnimation(L"animation\\Pet\\ChocoDrop\\ChocoDrop_Idle.anim");
	GetAnimator()->LoadAnimation(L"animation\\Pet\\ChocoDrop\\ChocoDrop_Magnet.anim");
	GetAnimator()->LoadAnimation(L"animation\\Pet\\ChocoDrop\\ChocoDrop_Skill.anim");
	GetAnimator()->LoadAnimation(L"animation\\Pet\\ChocoDrop\\ChocoDrop_BonusEnter.anim");
	GetAnimator()->LoadAnimation(L"animation\\Pet\\ChocoDrop\\ChocoDrop_BonusIdle.anim");
	GetAnimator()->LoadAnimation(L"animation\\Pet\\ChocoDrop\\ChocoDrop_BonusJump.anim");
	GetAnimator()->LoadAnimation(L"animation\\Pet\\ChocoDrop\\ChocoDrop_BonusExit.anim");

	GetFSM()->AddState(L"Idle", new CPetIdleState);
	GetFSM()->AddState(L"Magnet", new CPetMagnetState);
	//GetFSM()->AddState(L"Skill", new CPetSkillState);
	GetFSM()->AddState(L"BonusEnter", new CPetBonusEnterState);
	GetFSM()->AddState(L"BonusIdle", new CPetBonusIdleState);
	GetFSM()->AddState(L"BonusJump", new CPetBonusJumpState);
	GetFSM()->AddState(L"BonusExit", new CPetBonusExitState);

	//처음 상태는 Idle
	GetFSM()->ChangeState(L"Idle");

	m_fSkillAccTime = 0;
	m_fSkillCoolTime = 0;
	SetScale(Vec2(70.f, 70.f));
}

CPetChocoDrop::~CPetChocoDrop()
{
}

void CPetChocoDrop::tick()
{
	CPet::tick();
}

void CPetChocoDrop::render(HDC _dc)
{
	CPet::render(_dc);
}
