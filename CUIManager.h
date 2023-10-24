#pragma once

class CUI;

class CUIManager
{
	SINGLETON(CUIManager);

private:
	CUI*	m_pPriorityUI;		//���콺Ŭ�� �̺�Ʈ�� �޾ư� �ֿ켱���� UI
	CUI*	m_pFocusedUI;		//���� �����ؾ��ϴ�, ������ ������ UI

public:
	void	tick();

private:
	//�ֻ��� �θ� UI����, �ڽ� UI����
	CUI* GetPriorityUI(CUI* _pParentUI);
};

