#include "pch.h"
#include "CBearJellyParty.h"
#include "CResourceManager.h"
#include "CAnimator.h"
#include "CAnimation.h"

CBearJellyParty::CBearJellyParty()
{
	GetAnimator()->CreateAnimation(L"BearJellyParty", m_pTexture, Vec2(0.f, 450.f), Vec2(90.f, 90.f), Vec2(0.f, 0.f), 4, 0.1f);
	GetAnimator()->FindAnimation(L"BearJellyParty")->Save(L"animation\\Item_BearJellyParty.anim");
	GetAnimator()->LoadAnimation(L"animation\\Item_BearJellyParty.anim");
	GetAnimator()->Play(L"BearJellyParty", true);	
}

CBearJellyParty::~CBearJellyParty()
{
}

void CBearJellyParty::render(HDC _dc)
{
	CObj::render(_dc);
}

void CBearJellyParty::BeginOverlap(CCollider* _pOther)
{
}

void CBearJellyParty::OnOverlap(CCollider* _pOther)
{
}

void CBearJellyParty::EndOverlap(CCollider* _pOther)
{
}
