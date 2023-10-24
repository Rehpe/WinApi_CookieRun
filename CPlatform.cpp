#include "pch.h"
#include "CPlatform.h"
#include "CResourceManager.h"
#include "CTexture.h"
#include "CCollider.h"
#include "CCamera.h"

CPlatform::CPlatform()
{
}

CPlatform::~CPlatform()
{
}

void CPlatform::render(HDC _dc)
{
	Vec2 vPos = CCamera::GetInst()->GetRenderPos(GetPos());
	Vec2 vScale = GetScale();

	//AlphaBlend ¼³Á¤
	BLENDFUNCTION tBlend = {};

	tBlend.AlphaFormat = AC_SRC_ALPHA;
	tBlend.BlendFlags = 0;
	tBlend.BlendOp = AC_SRC_OVER;
	tBlend.SourceConstantAlpha = int(255.f);

	AlphaBlend(_dc
		, (int)(vPos.x - m_pTexture->GetWidth() / 2)
		, (int)(vPos.y - m_pTexture->GetHeight() / 2)
		, m_pTexture->GetWidth()
		, m_pTexture->GetHeight()
		, m_pTexture->GetDC()
		, 0, 0
		, m_pTexture->GetWidth()
		, m_pTexture->GetHeight()
		, tBlend);

	CObj::render(_dc);
}

void CPlatform::BeginOverlap(CCollider* _pOther)
{
}

void CPlatform::OnOverlap(CCollider* _pOther)
{
}

void CPlatform::EndOverlap(CCollider* _pOther)
{
}
