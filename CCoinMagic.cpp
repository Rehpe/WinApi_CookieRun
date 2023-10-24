#include "pch.h"
#include "CCoinMagic.h"
#include "CResourceManager.h"
#include "CAnimator.h"
#include "CAnimation.h"

CCoinMagic::CCoinMagic()
{
	GetAnimator()->CreateAnimation(L"CoinMagic", m_pTexture, Vec2(0.f, 360.f), Vec2(90.f, 90.f), Vec2(0.f, 0.f), 4, 0.1f);
	GetAnimator()->LoadAnimation(L"animation\\Item_CoinMagic.anim");
	GetAnimator()->FindAnimation(L"CoinMagic")->Save(L"animation\\Item_CoinMagic.anim");
	GetAnimator()->Play(L"CoinMagic", true);
}

CCoinMagic::~CCoinMagic()
{
}

void CCoinMagic::render(HDC _dc)
{
	CObj::render(_dc);
}

void CCoinMagic::BeginOverlap(CCollider* _pOther)
{
}

void CCoinMagic::OnOverlap(CCollider* _pOther)
{
}

void CCoinMagic::EndOverlap(CCollider* _pOther)
{
}
