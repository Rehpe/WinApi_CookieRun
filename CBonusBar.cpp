#include "pch.h"
#include "CBonusBar.h"
#include "CResourceManager.h"
#include "CEngine.h"
#include "CTexture.h"
#include "CLevelManager.h"
#include "CLevel.h"
#include "CPlayer.h"

CBonusBar::CBonusBar()
{
	m_pBGTex = CResourceManager::GetInst()->LoadTexture(L"BonusBG", L"texture\\UI\\BonusBG.png");
	m_pBarTex = CResourceManager::GetInst()->LoadTexture(L"BonusBar", L"texture\\UI\\BonusBar.png");

	Vec2 vResolution = CEngine::GetInst()->GetResolution();
	SetPos(Vec2(20.f, 30.f));

	SetMaxValue(CLevelManager::GetInst()->GetCurLevel()->GetPlayer()->GetBonusMaxTime());
	SetVarValue(CLevelManager::GetInst()->GetCurLevel()->GetPlayer()->GetBonusAccTime());
	m_vBarPos = GetPos();
}

CBonusBar::~CBonusBar()
{
}

void CBonusBar::tick()
{
	SetMaxValue(CLevelManager::GetInst()->GetCurLevel()->GetPlayer()->GetBonusMaxTime());
	SetVarValue(CLevelManager::GetInst()->GetCurLevel()->GetPlayer()->GetBonusAccTime());

	SetRatio(1 - GetVarValue() / GetMaxValue());

	if (CLevelManager::GetInst()->GetCurLevel()->GetPlayer()->GetBonusAccTime() == 0)
		SetRatio(0);

	CProgressBarUI::tick();
}

void CBonusBar::render(HDC _dc)
{
	CProgressBarUI::render(_dc);
}
