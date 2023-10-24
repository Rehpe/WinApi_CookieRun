#include "pch.h"
#include "CPreview.h"
#include "CKeyManager.h"
#include "CTexture.h"

CPreview::CPreview()
	:m_pPreviewTex(nullptr)
{
}

CPreview::~CPreview()
{
}

void CPreview::render(HDC _dc)
{
	Vec2 vPos = CKeyManager::GetInst()->GetMousePos();
	Vec2 vScale = Vec2(m_pPreviewTex->GetWidth(),m_pPreviewTex->GetHeight());

	if (m_pPreviewTex)
	{	
		//AlphaBlend ¼³Á¤
		BLENDFUNCTION tBlend = {};

		tBlend.AlphaFormat = AC_SRC_ALPHA;
		tBlend.BlendFlags = 0;
		tBlend.BlendOp = AC_SRC_OVER;
		tBlend.SourceConstantAlpha = int(200.f);

		AlphaBlend(_dc
			, (int)(vPos.x - vScale.x/2)
			, (int)(vPos.y - vScale.y/2)
			, m_pPreviewTex->GetWidth()
			, m_pPreviewTex->GetHeight()
			, m_pPreviewTex->GetDC()
			, 0, 0
			, m_pPreviewTex->GetWidth()
			, m_pPreviewTex->GetHeight()
			, tBlend);
	}

	CObj::render(_dc);
}
