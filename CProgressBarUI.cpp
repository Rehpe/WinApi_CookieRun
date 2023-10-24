#include "pch.h"
#include "CProgressBarUI.h"
#include "CTexture.h"

CProgressBarUI::CProgressBarUI()
{
}

CProgressBarUI::~CProgressBarUI()
{
}

void CProgressBarUI::tick()
{
	CUI::tick();
}

void CProgressBarUI::render(HDC _dc)
{
	BG_render(_dc);
	Bar_render(_dc);
	Tail_render(_dc);

	render_childUI(_dc);
}

void CProgressBarUI::BG_render(HDC _dc)
{

	//AlphaBlend 설정
	BLENDFUNCTION tBlend = {};

	tBlend.AlphaFormat = AC_SRC_ALPHA;
	tBlend.BlendFlags = 0;
	tBlend.BlendOp = AC_SRC_OVER;
	tBlend.SourceConstantAlpha = int(255.f);

	AlphaBlend(_dc
		, (int)(GetPos().x)
		, (int)(GetPos().y)
		, m_pBGTex->GetWidth()
		, m_pBGTex->GetHeight()
		, m_pBGTex->GetDC()
		, 0, 0
		, m_pBGTex->GetWidth()
		, m_pBGTex->GetHeight()
		, tBlend);
}

void CProgressBarUI::Bar_render(HDC _dc)
{
	//AlphaBlend 설정
	BLENDFUNCTION tBlend = {};

	tBlend.AlphaFormat = AC_SRC_ALPHA;
	tBlend.BlendFlags = 0;
	tBlend.BlendOp = AC_SRC_OVER;
	tBlend.SourceConstantAlpha = int(255.f);


	AlphaBlend(_dc
		, (int)(m_vBarPos.x)
		, (int)(m_vBarPos.y)
		, m_pBarTex->GetWidth() * GetRatio()
		, m_pBarTex->GetHeight()
		, m_pBarTex->GetDC()
		, 0, 0
		, m_pBarTex->GetWidth() * GetRatio()
		, m_pBarTex->GetHeight()
		, tBlend);
}

void CProgressBarUI::Tail_render(HDC _dc)
{
	if (m_pTailTex == nullptr)
		return;

	//AlphaBlend 설정
	BLENDFUNCTION tBlend = {};

	tBlend.AlphaFormat = AC_SRC_ALPHA;
	tBlend.BlendFlags = 0;
	tBlend.BlendOp = AC_SRC_OVER;
	tBlend.SourceConstantAlpha = int(255.f);

	AlphaBlend(_dc
		, (int)(m_vBarTail.x)
		, (int)(m_vBarTail.y)
		, m_pTailTex->GetWidth()
		, m_pTailTex->GetHeight()
		, m_pTailTex->GetDC()
		, 0, 0
		, m_pTailTex->GetWidth()
		, m_pTailTex->GetHeight()
		, tBlend);
}
