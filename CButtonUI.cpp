#include "pch.h"
#include "CButtonUI.h"

#include "CLevelManager.h"
#include "CEditorLevel.h"

#include "CTexture.h"

CButtonUI::CButtonUI()
	: m_pFunc(nullptr)
	, m_pPressedTex(nullptr)
	, m_pHighlightTex(nullptr)
	, m_pInst(nullptr)
	, m_pDelegateFunc(nullptr)
	, m_pDoubleParamDelegateFunc(nullptr)
{
}

CButtonUI::~CButtonUI()
{
}

void CButtonUI::render(HDC _dc)
{
	CTexture* pTargetTex = GetIdleTex();

	if (IsLbtnDown())
	{
		if (m_pPressedTex)
			pTargetTex = m_pPressedTex;
	}
	else if (IsMouseOn())
	{
		if (m_pHighlightTex)
			pTargetTex = m_pHighlightTex;
	}

	if (!pTargetTex)
	{
		//텍스쳐가 없다면 BTN_SIZE 크기만큼의 사각형 버튼 출력
		
		Rectangle(_dc
			, int(GetFinalPos().x)
			, int(GetFinalPos().y)
			, int(GetFinalPos().x + BTN_SIZE)
			, int(GetFinalPos().y + BTN_SIZE));

		CUI::render(_dc);
		return;
	}

	Vec2 vPos = GetFinalPos();
	//AlphaBlend 설정
	BLENDFUNCTION tBlend = {};

	tBlend.AlphaFormat = AC_SRC_ALPHA;
	tBlend.BlendFlags = 0;
	tBlend.BlendOp = AC_SRC_OVER;
	tBlend.SourceConstantAlpha = int(255.f);

	AlphaBlend(_dc
		, (int)(vPos.x)
		, (int)(vPos.y)
		, pTargetTex->GetWidth()
		, pTargetTex->GetHeight()
		, pTargetTex->GetDC()
		, 0, 0
		, pTargetTex->GetWidth()
		, pTargetTex->GetHeight()
		, tBlend);

	render_childUI(_dc);
}

void CButtonUI::MouseLbtnClicked()
{

	BtnClickSound();

	if (nullptr != m_pFunc)
		m_pFunc();

	if (m_pInst && m_pDelegateFunc)
	{
		(m_pInst->*m_pDelegateFunc)();
	}

	if (m_pInst && m_pDoubleParamDelegateFunc)
	{
		(m_pInst->*m_pDoubleParamDelegateFunc)(m_strRelativePath,m_iStartCount);
	}

}
