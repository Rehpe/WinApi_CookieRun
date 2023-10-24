#include "pch.h"
#include "CPanelUI.h"
#include "CKeyManager.h"
#include "CTexture.h"

CPanelUI::CPanelUI()
	:m_fRatio(1)
{
}

CPanelUI::~CPanelUI()
{
}

void CPanelUI::tick()
{
	//드래그 앤 드롭 기능
	//if (IsLbtnDown())
	//{
	//	Vec2 vMove = MOUSE_POS - m_vPressPos;

	//	Vec2 vPos = GetPos();
	//	vPos += vMove;
	//	SetPos(vPos);

	//	m_vPressPos = MOUSE_POS;
	//}

	CUI::tick();
}

void CPanelUI::render(HDC _dc)
{
	//if (nullptr == GetIdleTex())
	//	return;

	if(GetIdleTex()!=nullptr)
	{
		Vec2 vPos = GetFinalPos();

		//AlphaBlend 설정
		BLENDFUNCTION tBlend = {};

		tBlend.AlphaFormat = AC_SRC_ALPHA;
		tBlend.BlendFlags = 0;
		tBlend.BlendOp = AC_SRC_OVER;
		tBlend.SourceConstantAlpha = int(255.f) * m_fRatio;

		AlphaBlend(_dc
			, (int)(vPos.x)
			, (int)(vPos.y)
			, GetIdleTex()->GetWidth()
			, GetIdleTex()->GetHeight()
			, GetIdleTex()->GetDC()
			, 0, 0
			, GetIdleTex()->GetWidth()
			, GetIdleTex()->GetHeight()
			, tBlend);
	}

	CObj::render(_dc);

	render_childUI(_dc);

}

void CPanelUI::MouseLbtnDown()
{
	CUI::MouseLbtnDown();

	m_vPressPos = MOUSE_POS;
}
//
//void CPanelUI::MouseLbtnClicked()
//{
//}
