#include "pch.h"
#include "CMiniPotion.h"
#include "CResourceManager.h"
#include "CAnimator.h"
#include "CAnimation.h"

CMiniPotion::CMiniPotion()
{
	GetAnimator()->CreateAnimation(L"MiniPotion", m_pTexture, Vec2(0.f, 540.f), Vec2(90.f, 90.f), Vec2(0.f, 0.f), 4, 0.1f);
	GetAnimator()->LoadAnimation(L"animation\\Item_MiniPotion.anim");
	GetAnimator()->FindAnimation(L"MiniPotion")->Save(L"animation\\Item_MiniPotion.anim");
	GetAnimator()->Play(L"MiniPotion", true);
}

CMiniPotion::~CMiniPotion()
{
}

void CMiniPotion::render(HDC _dc)
{
}

void CMiniPotion::BeginOverlap(CCollider* _pOther)
{
}

void CMiniPotion::OnOverlap(CCollider* _pOther)
{
}

void CMiniPotion::EndOverlap(CCollider* _pOther)
{
}
