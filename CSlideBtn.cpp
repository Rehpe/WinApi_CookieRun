#include "pch.h"
#include "CSlideBtn.h"
#include "CResourceManager.h"
#include "CTexture.h"
#include "CKeyManager.h"
#include "CEngine.h"

CSlideBtn::CSlideBtn()
{
	SetIdleTex(CResourceManager::GetInst()->LoadTexture(L"SlideBtn", L"texture\\UI\\SlideBtn.png"));
	SetPressedTex(CResourceManager::GetInst()->LoadTexture(L"SlideBtn_Pressed", L"texture\\UI\\SlideBtn_Pressed.png"));

	Vec2 vResolution = CEngine::GetInst()->GetResolution();
	SetPos(Vec2(vResolution.x - 235.f, vResolution.y - 200.f));
}

CSlideBtn::~CSlideBtn()
{
}

void CSlideBtn::tick()
{
	CUI::tick();

	if (IsPressed(KEY::DOWN))
	{
		SetLbtnDown(true);
	}
	if (IsRelease(KEY::DOWN))
	{
		SetLbtnDown(false);
	}
}

void CSlideBtn::render(HDC _dc)
{
	CButtonUI::render(_dc);
}
