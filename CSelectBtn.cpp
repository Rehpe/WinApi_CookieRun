#include "pch.h"
#include "CSelectBtn.h"
#include "CTexture.h"
#include "CSelectLevel.h"
#include "CLevelManager.h"
#include "CLevel.h"
#include "CPanelUI.h"
#include "CAnimator.h"

CSelectBtn::CSelectBtn()
	:m_eCookieType(SELECT_COOKIE::NONE)
	,m_ePetType(SELECT_PET::NONE)
{
}

CSelectBtn::~CSelectBtn()
{
}

void CSelectBtn::render(HDC _dc)
{
	CTexture* pTargetTex = GetIdleTex();

	HBRUSH	hNullBrush = (HBRUSH)GetStockObject(NULL_BRUSH);
	HBRUSH hDefaultBrush = (HBRUSH)SelectObject(_dc, hNullBrush);

	if (!pTargetTex)
	{
		//텍스쳐가 없다면 BtnSize 크기만큼의 사각형 버튼 출력
		const int BtnSize = 119;

		Rectangle(_dc
			, int(GetFinalPos().x)
			, int(GetFinalPos().y)
			, int(GetFinalPos().x + BtnSize)
			, int(GetFinalPos().y + BtnSize));

		CUI::render(_dc);
		return;
	}
	SelectObject(_dc, hDefaultBrush);

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
		, BTN_SIZE
		, BTN_SIZE
		, GetIdleTex()->GetDC()
		, 0, 0
		, GetIdleTex()->GetWidth()
		, GetIdleTex()->GetHeight()
		, tBlend);

	render_childUI(_dc);
}

void CSelectBtn::selectObject()
{
	CSelectLevel* pCurLevel = dynamic_cast<CSelectLevel*>(CLevelManager::GetInst()->GetCurLevel());

	//플레이어 버튼일 경우
	if (m_ePetType == SELECT_PET::NONE)
	{
		//SelectLevel의 플레이어 정보에 본인의 쿠키 정보 대입
		pCurLevel->SetCookieType(m_eCookieType);

		switch (m_eCookieType)
		{
		case SELECT_COOKIE::NONE:
			break;
		case SELECT_COOKIE::GINGERBRAVE:
		{
			pCurLevel->GetCookieAnim()->GetAnimator()->Play(L"GingerBrave", true);
		}
		break;
		case SELECT_COOKIE::MILK:
		{
			pCurLevel->GetCookieAnim()->GetAnimator()->Play(L"Milk", true);
		}
		break;
		case SELECT_COOKIE::FAIRY:
		{
			pCurLevel->GetCookieAnim()->GetAnimator()->Play(L"Fairy", true);
		}
		break;
		case SELECT_COOKIE::SNOWSUGAR:
		{
			pCurLevel->GetCookieAnim()->GetAnimator()->Play(L"SnowSugar", true);
		}
		break;
		default:
			break;
		}
	}

	//펫 버튼일 경우
	if (m_eCookieType == SELECT_COOKIE::NONE)
	{
		pCurLevel->SetPetType(m_ePetType);

		switch (m_ePetType)
		{
		case SELECT_PET::NONE:
			break;
		case SELECT_PET::CHOCODROP:
		{
			pCurLevel->GetPetAnim()->GetAnimator()->Play(L"ChocoDrop", true);
		}
			break;
		case SELECT_PET::MILKBOTTLE:
		{
			pCurLevel->GetPetAnim()->GetAnimator()->Play(L"MilkBottle", true);
		}
			break;
		case SELECT_PET::FLOWERPOD:
		{
			pCurLevel->GetPetAnim()->GetAnimator()->Play(L"FlowerPod", true);
		}
		break;
		case SELECT_PET::SNOWGLOBE:
		{
			pCurLevel->GetPetAnim()->GetAnimator()->Play(L"SnowGlobe", true);
		}
		break;
		default:
			break;
		}
	}
}
