#include "pch.h"
#include "CUI.h"
#include "CTexture.h"
#include "CKeyManager.h"
#include "CCamera.h"

CUI::CUI()
	: m_bLbtnDown(false)
	, m_bMouseOn(false)
	, m_bCameraAffected(false)
	, m_pIdleTex(nullptr)
	, m_pParentUI(nullptr)
{
	SetDoNotDestroy(true);
}

CUI::CUI(const CUI& _other)
	: CObj(_other)
	, m_pParentUI(nullptr)
	, m_pIdleTex(_other.m_pIdleTex)
	, m_bLbtnDown(false)
	, m_bMouseOn(false)
	, m_bCameraAffected(_other.m_bCameraAffected)
{
	for (size_t i = 0; i < _other.m_vecChildUI.size(); ++i)
	{
		AddChildUI(_other.m_vecChildUI[i]->Clone());
	}
}


CUI::~CUI()
{
	for (size_t i = 0; i < m_vecChildUI.size(); i++)
	{
		DEL(m_vecChildUI[i]);
		
	}
	m_vecChildUI.clear();

}
	

void CUI::tick()
{
	// 1. 위치 계산

	//부모가 없을 경우 본인의 포지션이 곧 fianl Pos
	m_vFinalPos = GetPos();

	//부모 UI가 있다면
	if (m_pParentUI)
	{
		m_vFinalPos += m_pParentUI->GetFinalPos();
	}

	// 2. 마우스 포인터 체크
	MouseOnCheck();

	//Level은 최상위 UI만을 가지므로, 자식이 있는 UI는 자식들을 순회하며 자식 tick을 호출
	for (size_t i = 0; i < m_vecChildUI.size(); i++)
	{
		m_vecChildUI[i]->tick();
	}

	CObj::final_tick();
}

void CUI::render(HDC _dc)
{
	CObj::render(_dc);
	render_childUI(_dc); 
	
	
}

void CUI::render_childUI(HDC _dc)
{
	//Level은 최상위 UI만을 가지므로, 자식이 있는 UI는 자식들을 순회하며 자식 render을 호출
	for (size_t i = 0; i < m_vecChildUI.size(); i++)
	{
		m_vecChildUI[i]->render(_dc);
	}
}

void CUI::SetIdleTex(CTexture* _pTex)
{
	m_pIdleTex = _pTex;

	if (m_pIdleTex != nullptr)
	{
		Vec2 vScale = Vec2((float)m_pIdleTex->GetWidth(), (float)m_pIdleTex->GetHeight());
		SetScale(vScale);
	}
}

void CUI::ClearChildUI()
{
	for (size_t i = 0; i < m_vecChildUI.size(); i++)
	{
		delete m_vecChildUI[i];
	}

	m_vecChildUI.clear();
}

void CUI::MouseOnCheck()
{
	Vec2 vPos = GetFinalPos();
	Vec2 vScale = GetScale();
	Vec2 vMousePos = MOUSE_POS;

	//카메라 영향을 받는 경우 마우스 좌표를 real Pos로 변환한다
	if (m_bCameraAffected)
	{
		vMousePos = CCamera::GetInst()->GetRealPos(vMousePos);
	}

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
