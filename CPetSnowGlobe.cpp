#include "pch.h"
#include "CPetSnowGlobe.h"
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
#include "CSnowGlobeSkillState.h"

CPetSnowGlobe::CPetSnowGlobe()
{
	//아틀라스 텍스처 Load
	SetPetTex(CResourceManager::GetInst()->LoadTexture(L"SnowGlobe_Atlas", L"texture\\pet\\SnowGlobe_Atlas.png"));

	//애니메이션 Load 
	GetAnimator()->LoadAnimation(L"animation\\Pet\\SnowGlobe\\SnowGlobe_Idle.anim");
	GetAnimator()->LoadAnimation(L"animation\\Pet\\SnowGlobe\\SnowGlobe_Magnet.anim");
	GetAnimator()->LoadAnimation(L"animation\\Pet\\SnowGlobe\\SnowGlobe_Skill.anim");
	GetAnimator()->LoadAnimation(L"animation\\Pet\\SnowGlobe\\SnowGlobe_BonusEnter.anim");
	GetAnimator()->LoadAnimation(L"animation\\Pet\\SnowGlobe\\SnowGlobe_BonusIdle.anim");
	GetAnimator()->LoadAnimation(L"animation\\Pet\\SnowGlobe\\SnowGlobe_BonusJump.anim");
	GetAnimator()->LoadAnimation(L"animation\\Pet\\SnowGlobe\\SnowGlobe_BonusExit.anim");

	GetFSM()->AddState(L"Idle", new CPetIdleState);
	GetFSM()->AddState(L"Magnet", new CPetMagnetState);
	GetFSM()->AddState(L"Skill", new CSnowGlobeSkillState);
	GetFSM()->AddState(L"BonusEnter", new CPetBonusEnterState);
	GetFSM()->AddState(L"BonusIdle", new CPetBonusIdleState);
	GetFSM()->AddState(L"BonusJump", new CPetBonusJumpState);
	GetFSM()->AddState(L"BonusExit", new CPetBonusExitState);

	//처음 상태는 Idle
	GetFSM()->ChangeState(L"Idle");

	m_pMakeJellySFX = CResourceManager::GetInst()->LoadSound(L"SnowSugar_Pet_Make_Jelly", L"sound\\Cookie\\SnowSugar_Pet_Make_Jelly.wav");

	m_fSkillCoolTime = 15.f;
	SetScale(Vec2(70.f, 70.f));
}

CPetSnowGlobe::~CPetSnowGlobe()
{
}

void CPetSnowGlobe::tick()
{
	CPet::tick();
}

void CPetSnowGlobe::render(HDC _dc)
{
	CPet::render(_dc);
}
