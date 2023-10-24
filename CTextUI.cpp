#include "pch.h"
#include "CTextUI.h"
#include "CPathManager.h"
#include "CLevelManager.h"
#include "CPlayer.h"
#include "CLevel.h"
#include "CEngine.h"
#include "CResourceManager.h"
#include "CTexture.h"
#include <atlstr.h>

CTextUI::CTextUI()
	: m_iNumber(0)
	, m_iFontSize(40)
	, m_eFontAlign(FONT_ALIGN::CENTER)
{
	//폰트 설정
	wstring strFilePath = CPathManager::GetInst()->GetContentPath();
	strFilePath += wstring(L"CookieRun_Bold.ttf");
	AddFontResourceW(strFilePath.c_str());
}

CTextUI::~CTextUI()
{
}

void CTextUI::tick()
{
 //	SetNumber(m_iNumber);
	CUI::tick();
}

void CTextUI::render(HDC _dc)
{
	//텍스트 가운데 정렬
	switch (m_eFontAlign)
	{
	case FONT_ALIGN::LEFT:
	{
		SetTextAlign(_dc, TA_LEFT);
	}
		break;
	case FONT_ALIGN::CENTER:
	{
		SetTextAlign(_dc, TA_CENTER);
	}
		break;
	case FONT_ALIGN::RIGHT:
	{
		SetTextAlign(_dc, TA_RIGHT);
	}
		break;
	default:
		break;
	}

	//폰트 기본 설정
	SetBkMode(_dc, 1);
	SetTextColor(_dc, RGB(0, 0, 0));
	HFONT hFont, oldFont;
	hFont = CreateFont(m_iFontSize, 0, 0, 0, 0, 0, 0, 0, HANGUL_CHARSET, 0, 0, 0, VARIABLE_PITCH || FF_ROMAN, L"CookieRun Bold");
	oldFont = (HFONT)SelectObject(_dc, hFont);
	DeleteObject(oldFont);

	//점수에 콤마찍어서 str로 변환
	std::wstring str = NumberComma(m_iNumber);

	//외곽선 텍스트 출력
	for (int i = -3; i <= 3; i++)
		for (int j = -3; j <= 3; j++)
			TextOut(_dc, GetPos().x + i, GetPos().y + j, str.c_str(), str.size());
	SetTextColor(_dc, RGB(255, 255, 255));
	TextOut(_dc, GetPos().x, GetPos().y, str.c_str(), str.size());


}

wstring CTextUI::NumberComma(int _num)
{
	int n = 0, count = 0;
	CString strRetrunValue = L"";

	if (_num == 0)
		return L"0";

	while (_num > 0)
	{
		count++;
		n = _num % 10;
		_num = _num / 10;

		strRetrunValue.Format(L"%d" + strRetrunValue, n);

		if ((count % 3 == 0) && (_num > 0))
			strRetrunValue.Format(L"," + strRetrunValue);
	}
	return (wstring)strRetrunValue;
}
