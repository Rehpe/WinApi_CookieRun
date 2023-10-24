#include "pch.h"
#include "CCookieSkillBar.h"
#include "CResourceManager.h"
#include "CLevelManager.h"
#include "CPlayer.h"
#include "CLevel.h"
#include "CFSM.h"
#include "CState.h"


CCookieSkillBar::CCookieSkillBar()
{
	m_pBGTex = CResourceManager::GetInst()->LoadTexture(L"CookieSkillBG", L"texture\\UI\\CookieSkillBG.png");
	m_pBarTex = CResourceManager::GetInst()->LoadTexture(L"CookieSkillBar", L"texture\\UI\\CookieSkillBar.png");

    //SetMaxValue(CLevelManager::GetInst()->GetCurLevel()->GetPlayer()->GetSkillCoolTime());
	//SetVarValue(CLevelManager::GetInst()->GetCurLevel()->GetPlayer()->GetSkillAccTime());
	m_vBarPos = GetPos();
}

CCookieSkillBar::~CCookieSkillBar()
{
}

void CCookieSkillBar::tick()
{
	SetMaxValue(CLevelManager::GetInst()->GetCurLevel()->GetPlayer()->GetSkillCoolTime());
	SetVarValue(CLevelManager::GetInst()->GetCurLevel()->GetPlayer()->GetSkillAccTime());
	m_vBarPos = GetPos();

	SetRatio(GetVarValue() / GetMaxValue());

	CProgressBarUI::tick();
}

void CCookieSkillBar::render(HDC _dc)
{
	if (CLevelManager::GetInst()->GetCurLevel()->GetPlayer()->GetFSM()->GetCurState()->GetKey() == L"Skill")	
		return;

	if (CLevelManager::GetInst()->GetCurLevel()->GetPlayer()->GetSkillAccTime() == 0)
		return;

	if (CLevelManager::GetInst()->GetCurLevel()->GetPlayer()->IsBonus())
		return;


	CProgressBarUI::render(_dc);
}
