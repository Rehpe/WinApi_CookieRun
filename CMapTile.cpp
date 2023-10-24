#include "pch.h"
#include "CMapTile.h"
#include "CCollider.h"
#include "CResourceManager.h"
#include "CTexture.h"
#include "CCamera.h"
#include "CKeyManager.h"
#include "CLevelManager.h"
#include "CLevel.h"
#include "CEditorLevel.h"
#include "CKeyManager.h"

CMapTile::CMapTile()
	:m_pTexture(nullptr)
{
	CreateCollider();

	
}

CMapTile::~CMapTile()
{
}

void CMapTile::init()
{
	if(m_pTexture)
	{
		SetScale(Vec2(m_pTexture->GetWidth(), m_pTexture->GetHeight()));
	}
}

void CMapTile::tick()
{
	CEditorLevel* pLevel = dynamic_cast<CEditorLevel*>(CLevelManager::GetInst()->GetCurLevel());
	if (pLevel == nullptr)
		return;

	MouseOnCheck();

	if (m_bMouseOn && IsTap(KEY::RBTN))
	{
		SetDead();
	}
}

void CMapTile::render(HDC _dc)
{
}

void CMapTile::BeginOverlap(CCollider* _pOther)
{
}

void CMapTile::OnOverlap(CCollider* _pOther)
{
}

void CMapTile::EndOverlap(CCollider* _pOther)
{
}

void CMapTile::MouseOnCheck()
{
	Vec2 vPos = GetPos();
	Vec2 vScale = GetScale();
	Vec2 vMousePos = MOUSE_POS;

	vMousePos = CCamera::GetInst()->GetRealPos(vMousePos);
	

	if (vPos.x <= vMousePos.x && vMousePos.x <= vPos.x + vScale.x
		&& vPos.y <= vMousePos.y && vMousePos.y <= vPos.y + vScale.y)
	{
		m_bMouseOn = true;
	}
	else
	{
		m_bMouseOn = false;
	}
}
