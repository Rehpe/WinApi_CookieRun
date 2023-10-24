#include "pch.h"
#include "CBooster.h"
#include "CResourceManager.h"
#include "CAnimator.h"
#include "CAnimation.h"

CBooster::CBooster()
{
	GetAnimator()->CreateAnimation(L"Booster", m_pTexture, Vec2(0.f, 90.f), Vec2(90.f, 90.f), Vec2(0.f, 0.f), 4, 0.1f);
	GetAnimator()->LoadAnimation(L"animation\\Item_Booster.anim");
	GetAnimator()->FindAnimation(L"Booster")->Save(L"animation\\Item_Booster.anim");
	GetAnimator()->Play(L"Booster", true);
}

CBooster::~CBooster()
{
}

void CBooster::render(HDC _dc)
{
	CObj::render(_dc);
}

void CBooster::BeginOverlap(CCollider* _pOther)
{
}

void CBooster::OnOverlap(CCollider* _pOther)
{
}

void CBooster::EndOverlap(CCollider* _pOther)
{
}
