#pragma once

class CUI;

class CUIManager
{
	SINGLETON(CUIManager);

private:
	CUI*	m_pPriorityUI;		//마우스클릭 이벤트를 받아갈 최우선순위 UI
	CUI*	m_pFocusedUI;		//현재 반응해야하는, 초점이 맞춰진 UI

public:
	void	tick();

private:
	//최상위 부모 UI포함, 자식 UI까지
	CUI* GetPriorityUI(CUI* _pParentUI);
};

