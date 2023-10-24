#include "pch.h"
#include "CItem.h"
#include "CAnimator.h"
#include "CPlayer.h"
#include "CCollider.h"
#include "CResourceManager.h"

CItem::CItem()
{
	CreateAnimator();
	m_pTexture = CResourceManager::GetInst()->LoadTexture(L"items", L"texture\\item\\items.png");
	GetCollider()->SetScale(Vec2(70.f, 70.f));
	
}

CItem::~CItem()
{
}

void CItem::render(HDC _dc)
{
}

void CItem::BeginOverlap(CCollider* _pOther)
{
	//다이나믹캐스트를 이용하여 충돌한 오브젝트가 player 클래스인지 확인할 수 있다
	CPlayer* pPlayer = dynamic_cast<CPlayer*>(_pOther->GetOwner());
	if (pPlayer == nullptr)
		return;

}

void CItem::OnOverlap(CCollider* _pOther)
{
}

void CItem::EndOverlap(CCollider* _pOther)
{
}
