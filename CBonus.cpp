#include "pch.h"
#include "CBonus.h"
#include "CResourceManager.h"
#include "CAnimator.h"
#include "CAnimation.h"

CBonus::CBonus()
{
	GetAnimator()->CreateAnimation(L"Bonus", m_pTexture, Vec2(0.f, 0.f), Vec2(90.f, 90.f), Vec2(0.f, 0.f), 4, 0.1f);
	GetAnimator()->LoadAnimation(L"animation\\Item_Bonus.anim");
	GetAnimator()->FindAnimation(L"Bonus")->Save(L"animation\\Item_Bonus.anim");
	GetAnimator()->Play(L"Bonus", true);
}


CBonus::~CBonus()
{
}

void CBonus::render(HDC _dc)
{
	CObj::render(_dc);
}

void CBonus::BeginOverlap(CCollider* _pOther)
{
}

void CBonus::OnOverlap(CCollider* _pOther)
{
}

void CBonus::EndOverlap(CCollider* _pOther)
{
}
