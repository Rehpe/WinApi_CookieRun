#include "pch.h"
#include "CBackgroundEffect.h"
#include "CTexture.h"
#include "CResourceManager.h"
#include "CTimeManager.h"
#include "CCamera.h"

CBackgroundEffect::CBackgroundEffect()
	:m_fRatio(1)
{
}

CBackgroundEffect::~CBackgroundEffect()
{
}

void CBackgroundEffect::init(BACKGROUND_EFFECT _effect)
{
	switch (_effect)
	{
	case BACKGROUND_EFFECT::SEMITRANSPARENT:
	{
		CTexture* pTex = CResourceManager::GetInst()->LoadTexture(L"SemiTransparent_BG", L"texture\\UI\\SemiTransparent_BG.png");
		SetEffectTex(pTex);
	}
		break;
	case BACKGROUND_EFFECT::LOWHP:
	{
		CTexture* pTex = CResourceManager::GetInst()->LoadTexture(L"LowHP_BG", L"texture\\UI\\LowHP_BG.png");
		SetEffectTex(pTex);
	}
		break;
	default:
		break;
	}
}

void CBackgroundEffect::render(HDC _dc)
{
	if (!m_pTexture)
		return;

	m_fAccTime += DT;

	Vec2 vScale = Vec2(m_pTexture->GetWidth(), m_pTexture->GetHeight());

	if (m_bAlphaReduce)
	{
		m_fRatio = 1 - (m_fAccTime / m_fMaxTime);
	}

	if (m_bBlink)
	{
		//깜빡거리게 만든다
		static float Dir = 1.f;			//비율을 올릴것인지 내릴 것인지
		m_fRatio += DT * Dir * 2.f;

		if (1.f < m_fRatio)
		{
			m_fRatio = 1.f;
			Dir = -1.f;
		}
		else if (m_fRatio < 0.f)
		{
			m_fRatio = 0.f;
			Dir = 1;
		}

	}

	//AlphaBlend 설정
	BLENDFUNCTION tBlend = {};

	tBlend.AlphaFormat = AC_SRC_ALPHA;
	tBlend.BlendFlags = 0;
	tBlend.BlendOp = AC_SRC_OVER;
	tBlend.SourceConstantAlpha = int(255.f) * m_fRatio;

	AlphaBlend(_dc
		, (int)(GetPos().x)
		, (int)(GetPos().y)
		, m_pTexture->GetWidth()
		, m_pTexture->GetHeight()
		, m_pTexture->GetDC()
		, 0, 0
		, m_pTexture->GetWidth()
		, m_pTexture->GetHeight()
		, tBlend);

	//if (m_fAccTime >= m_fMaxTime)
		//SetDead();
}
