#include "pch.h"
#include "CJumpBtn.h"
#include "CResourceManager.h"
#include "CTexture.h"
#include "CKeyManager.h"
#include "CEngine.h"

CJumpBtn::CJumpBtn()
{
	SetIdleTex(CResourceManager::GetInst()->LoadTexture(L"JumpBtn", L"texture\\UI\\JumpBtn.png"));
	SetPressedTex(CResourceManager::GetInst()->LoadTexture(L"JumpBtn_Pressed", L"texture\\UI\\JumpBtn_Pressed.png"));

	Vec2 vResolution = CEngine::GetInst()->GetResolution();
	SetPos(Vec2(0.f, vResolution.y - 200.f));
}

CJumpBtn::~CJumpBtn()
{
}

void CJumpBtn::tick()
{
	CUI::tick();
	
	if (IsTap(KEY::SPACE))
	{
		SetLbtnDown(true);
	}
	if (IsRelease(KEY::SPACE))
	{
		SetLbtnDown(false);
	}
}

void CJumpBtn::render(HDC _dc)
{
	CButtonUI::render(_dc);
}
