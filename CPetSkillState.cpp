#include "pch.h"
#include "CPetSkillState.h"
#include "CObj.h"
#include "CFSM.h"
#include "CAnimation.h"
#include "CAnimator.h"
#include "CPet.h"
#include "CLevelManager.h"
#include "CLevel.h"
#include "CPlayer.h"

CPetSkillState::CPetSkillState()
{
}

CPetSkillState::~CPetSkillState()
{
}

void CPetSkillState::final_tick()
{
}

void CPetSkillState::Enter()
{
	//어떤 펫인지 알아낸다.
	CPet* pPet = dynamic_cast<CPet*>(GetOwnerFSM()->GetOwner());

	//펫이 아니라면 에러처리
	assert(pPet);

	//Skill 애니메이션을 재생한다
	GetOwnerFSM()->GetOwner()->GetAnimator()->Play(L"Skill", true);
}

void CPetSkillState::Exit()
{
	//해당 State 애니메이션을 Reset시킨다
	GetOwnerFSM()->GetOwner()->GetAnimator()->FindAnimation(L"Skill")->Reset();
}
