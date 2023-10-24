#include "pch.h"
#include "CHpBarUI.h"
#include "CTexture.h"
#include "CResourceManager.h"
#include "CEngine.h"
#include "CLevelManager.h"
#include "CPlayer.h"
#include "CLevel.h"
#include "CTimeManager.h"
#include "CBackgroundEffect.h"

CHpBarUI::CHpBarUI()
	: m_pEffect(nullptr)
{
	m_pBGTex = CResourceManager::GetInst()->LoadTexture(L"HpBG", L"texture\\UI\\HpBG.png");
	m_pBarTex = CResourceManager::GetInst()->LoadTexture(L"HpBar", L"texture\\UI\\HpBar.png");
	m_pTailTex = CResourceManager::GetInst()->LoadTexture(L"HpTail", L"texture\\UI\\HpTail.png");
	m_pEffect = CResourceManager::GetInst()->LoadTexture(L"HpEffect", L"texture\\UI\\HpEffect.png");

	Vec2 vResolution = CEngine::GetInst()->GetResolution();
	SetPos(Vec2((vResolution.x / 2.f) - (m_pBGTex->GetWidth() / 2.f), 10.f));
	m_vBarPos = GetPos() + Vec2(53.f, 0.f);

	SetMaxValue(CLevelManager::GetInst()->GetCurLevel()->GetPlayer()->GetMaxHP());
	SetVarValue(CLevelManager::GetInst()->GetCurLevel()->GetPlayer()->GetHP());
}

CHpBarUI::~CHpBarUI()
{
}

void CHpBarUI::tick()
{
	SetMaxValue(CLevelManager::GetInst()->GetCurLevel()->GetPlayer()->GetMaxHP());
	SetVarValue(CLevelManager::GetInst()->GetCurLevel()->GetPlayer()->GetHP());

	SetRatio(GetVarValue() / GetMaxValue());

	m_vBarTail = Vec2((float)(GetPos().x + 53.f + m_pBarTex->GetWidth() * GetRatio() - 10.f), (float)(GetPos().y + m_pBarTex->GetHeight() - 55.f));
	
	if (GetRatio() <= 0.1f)
	{
		if (m_pBGEffect == nullptr)
		{
			m_pBGEffect = new CBackgroundEffect;
			m_pBGEffect->init(BACKGROUND_EFFECT::LOWHP);
			m_pBGEffect->SetBlink(true);
			Instantiate(m_pBGEffect, Vec2(0.f, 0.f), LAYER::BACKGROUND_EFFECT);
		}
	}

	else
	{
		if (m_pBGEffect != nullptr)
		{
			m_pBGEffect->SetDead();
			m_pBGEffect = nullptr;
		}
	}

	CProgressBarUI::tick();
}

void CHpBarUI::render(HDC _dc)
{
	CProgressBarUI::render(_dc);
	Effect_render(_dc);
}

void CHpBarUI::Effect_render(HDC _dc)
{
	//남은 체력이 10% 이하일 경우
	if (GetRatio() <= 0.1f)
	{
		//플레이어 애니메이션을 깜빡거리게 만든다
		static float fRatio = 0.f;		//비율(0이 투명, 1이 불투명)
		static float Dir = 1.f;			//비율을 올릴것인지 내릴 것인지
		fRatio += DT * Dir * 3.f;

		if (1.f < fRatio)
		{
			fRatio = 1.f;
			Dir = -1.f;
		}
		else if (fRatio < 0.f)
		{
			fRatio = 0.f;
			Dir = 1;
		}

		//AlphaBlend 설정
		BLENDFUNCTION tBlend = {};

		tBlend.AlphaFormat = AC_SRC_ALPHA;
		tBlend.BlendFlags = 0;
		tBlend.BlendOp = AC_SRC_OVER;
		tBlend.SourceConstantAlpha = int(255.f) * fRatio;

		AlphaBlend(_dc
			, (int)(GetPos().x - (m_pEffect->GetWidth() / 2.f) + 43.f)
			, (int)(GetPos().y - (m_pEffect->GetHeight() / 2.f) + 44.f)
			, m_pEffect->GetWidth() * 0.8
			, m_pEffect->GetHeight() * 0.8
			, m_pEffect->GetDC()
			, 0, 0
			, m_pEffect->GetWidth()
			, m_pEffect->GetHeight()
			, tBlend);	
	}
}
