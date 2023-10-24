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
	// 1. ��ġ ���

	//�θ� ���� ��� ������ �������� �� fianl Pos
	m_vFinalPos = GetPos();

	//�θ� UI�� �ִٸ�
	if (m_pParentUI)
	{
		m_vFinalPos += m_pParentUI->GetFinalPos();
	}

	// 2. ���콺 ������ üũ
	MouseOnCheck();

	//Level�� �ֻ��� UI���� �����Ƿ�, �ڽ��� �ִ� UI�� �ڽĵ��� ��ȸ�ϸ� �ڽ� tick�� ȣ��
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
	//Level�� �ֻ��� UI���� �����Ƿ�, �ڽ��� �ִ� UI�� �ڽĵ��� ��ȸ�ϸ� �ڽ� render�� ȣ��
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

	//ī�޶� ������ �޴� ��� ���콺 ��ǥ�� real Pos�� ��ȯ�Ѵ�
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
