#include "pch.h"
#include "CIceBearJellyParty.h"
#include "CResourceManager.h"
#include "CAnimator.h"
#include "CAnimation.h"

CIceBearJellyParty::CIceBearJellyParty()
{
	GetAnimator()->CreateAnimation(L"IceBearJellyParty", m_pTexture, Vec2(0.f, 630.f), Vec2(90.f, 90.f), Vec2(0.f, 0.f), 4, 0.1f);
	GetAnimator()->LoadAnimation(L"animation\\Item_IceBearJellyParty.anim");
	GetAnimator()->FindAnimation(L"IceBearJellyParty")->Save(L"animation\\Item_IceBearJellyParty.anim");
	GetAnimator()->Play(L"IceBearJellyParty", true);
}

CIceBearJellyParty::~CIceBearJellyParty()
{
}

void CIceBearJellyParty::render(HDC _dc)
{
	CObj::render(_dc);
}

void CIceBearJellyParty::BeginOverlap(CCollider* _pOther)
{
}

void CIceBearJellyParty::OnOverlap(CCollider* _pOther)
{
}

void CIceBearJellyParty::EndOverlap(CCollider* _pOther)
{
}
