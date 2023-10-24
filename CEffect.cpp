#include "pch.h"
#include "CEffect.h"
#include "CTexture.h"
#include "CTimeManager.h"
#include "CCamera.h"
#include "CAnimator.h"
#include "CAnimation.h"

CEffect::CEffect()
	:m_fAccTime(0)
	, m_fSizeRatio(1)
	, m_Repeat(false)
{
}

CEffect::~CEffect()
{
}


void CEffect::render(HDC _dc)
{
	if (!GetAnimator())
	{
		m_fAccTime += DT;

		Vec2 vPos = CCamera::GetInst()->GetRenderPos(GetPos());
		Vec2 vScale = Vec2(m_pTexture->GetWidth(), m_pTexture->GetHeight());
		float fRatio = 1.f;

		if (m_bAlphaReduce)
		{
			fRatio = 1 - (m_fAccTime / m_fMaxTime);
		}

		//AlphaBlend ¼³Á¤
		BLENDFUNCTION tBlend = {};

		tBlend.AlphaFormat = AC_SRC_ALPHA;
		tBlend.BlendFlags = 0;
		tBlend.BlendOp = AC_SRC_OVER;
		tBlend.SourceConstantAlpha = int(255.f) * fRatio;

		AlphaBlend(_dc
			, (int)(vPos.x - m_pTexture->GetWidth() / 2)
			, (int)(vPos.y - m_pTexture->GetHeight() / 2)
			, m_pTexture->GetWidth() * m_fSizeRatio
			, m_pTexture->GetHeight() * m_fSizeRatio
			, m_pTexture->GetDC()
			, 0, 0
			, m_pTexture->GetWidth()
			, m_pTexture->GetHeight()
			, tBlend);


		if (m_fAccTime == 0 && m_fMaxTime == 0)
			return;

		if (m_fAccTime >= m_fMaxTime)
			SetDead();
	}

	if (GetAnimator())
	{
		if(GetAnimator()->GetCurAnimation()->IsFinish())
		{
			if(!m_Repeat)
				SetDead();
		}
	}

	CObj::render(_dc);
}
