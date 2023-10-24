#include "pch.h"
#include "CPageBtn.h"

CPageBtn::CPageBtn()
{
}

CPageBtn::~CPageBtn()
{
}

void CPageBtn::render(HDC _dc)
{
	CButtonUI::render(_dc);
}

void CPageBtn::MouseLbtnClicked()
{
	if (m_pInst && m_pDoubleParamDelegateFunc)
	{
		(m_pInst->*m_pDoubleParamDelegateFunc)(m_strRelativePath, m_iStartCount);
	}
}
