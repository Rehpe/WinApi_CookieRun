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
	//���̳���ĳ��Ʈ�� �̿��Ͽ� �浹�� ������Ʈ�� player Ŭ�������� Ȯ���� �� �ִ�
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
