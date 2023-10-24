#include "pch.h"
#include "CMagnet.h"
#include "CResourceManager.h"
#include "CAnimator.h"
#include "CAnimation.h"

CMagnet::CMagnet()
{
	GetAnimator()->CreateAnimation(L"Magnet", m_pTexture, Vec2(0.f,270.f), Vec2(90.f, 90.f), Vec2(0.f, 0.f), 4, 0.1f);
	GetAnimator()->LoadAnimation(L"animation\\Item_Magnet.anim");
	GetAnimator()->FindAnimation(L"Magnet")->Save(L"animation\\Item_Magnet.anim");
	GetAnimator()->Play(L"Magnet", true);
}

CMagnet::~CMagnet()
{
}

void CMagnet::render(HDC _dc)
{
	CObj::render(_dc);
}

void CMagnet::BeginOverlap(CCollider* _pOther)
{
}

void CMagnet::OnOverlap(CCollider* _pOther)
{
}

void CMagnet::EndOverlap(CCollider* _pOther)
{
}
