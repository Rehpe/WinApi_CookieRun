#include "pch.h"
#include "CBigger.h"
#include "CResourceManager.h"
#include "CAnimator.h"
#include "CAnimation.h"
#include "CCollider.h"

CBigger::CBigger()
{
	GetAnimator()->CreateAnimation(L"Bigger", m_pTexture, Vec2(0.f, 180.f), Vec2(90.f, 90.f), Vec2(0.f, 0.f), 4, 0.1f);
	GetAnimator()->LoadAnimation(L"animation\\Item_Bigger.anim");
	GetAnimator()->FindAnimation(L"Bigger")->Save(L"animation\\Item_Bigger.anim");
	GetAnimator()->Play(L"Bigger", true);

}

CBigger::~CBigger()
{
}

void CBigger::render(HDC _dc)
{
	CObj::render(_dc);
}

void CBigger::BeginOverlap(CCollider* _pOther)
{
}

void CBigger::OnOverlap(CCollider* _pOther)
{
}

void CBigger::EndOverlap(CCollider* _pOther)
{
}
