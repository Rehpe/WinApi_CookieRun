#include "pch.h"
#include "CUIManager.h"

#include "CUI.h"
#include "CLevelManager.h"
#include "CLevel.h"
#include "CKeyManager.h"

CUIManager::CUIManager()
	: m_pFocusedUI(nullptr)
	, m_pPriorityUI(nullptr)
{
}

CUIManager::~CUIManager()
{
}

void CUIManager::tick()
{
	bool bLbtnPressed = IsTap(KEY::LBTN);
	bool bLbtnReleased = IsRelease(KEY::LBTN);

	//���� ���� ���� �� UI Layer ��������
	CLevel* pCurLevel = CLevelManager::GetInst()->GetCurLevel();
	const vector<CObj*>& vecUI = pCurLevel->GetLayer(LAYER::UI);

	//���� ������ ���̾ �ֻ�ܿ� �������ǹǷ� FocusUI��� �����ϰ�, ������ �Ųٷκ��� ��ȸ
	for (int i = vecUI.size() - 1; i >= 0; i--)
	{
		//�θ� ������ �����Ͽ� �θ� ���� �ڽ� UI�� �̺�Ʈ�� ������ �ֿ켱���� UI�� ã�´�.
		m_pPriorityUI = GetPriorityUI((CUI*)vecUI[i]);

		if (m_pPriorityUI == nullptr)
			continue;

		//���ߵ� �ֿ켱���� UI�� ���콺�� �̺�Ʈ ȣ��
		m_pPriorityUI->MouseOn();

		//���콺 ���� ��ư�� ���ȴ�
		if (bLbtnPressed)
		{
			//LbtnDown �̺�Ʈ ȣ��(UI�� ���콺�� Ŭ���Ͽ� ������ �ִ� ����)
			m_pPriorityUI->MouseLbtnDown();

			// �θ�UI �� FocusedUI �� ����
			m_pFocusedUI = (CUI*)vecUI[i];
			pCurLevel->SetFocusedUI(m_pFocusedUI);
			break;
		}
		//���콺�� ������
		else if (bLbtnReleased)
		{
			//������ ���� ���� �ִ� (UI�� ������ �ִ� ���¿���)
			if (m_pPriorityUI->m_bLbtnDown == true)
			{
				//UI�� Ŭ���� ���̴�(Ŭ�� �̺�Ʈ ȣ��_
				m_pPriorityUI->MouseLbtnClicked();
				m_pPriorityUI->m_bLbtnDown = false;
			}
		}
	}
}

CUI* CUIManager::GetPriorityUI(CUI* _pParentUI)
{
	bool bLbtnReleased = IsRelease(KEY::LBTN);

	//pPRiorityUI �ʱ�ȭ �� ���Ͽ� ���� ����
	CUI* pPriorityUI = nullptr;

	//queue ���������� ���� �� �ʱ�ȭ
	static list<CUI*> queue;
	queue.clear();

	//Queue�� �θ� UI ����
	queue.push_back(_pParentUI);

	//ť�� ��������
	while (!queue.empty())
	{
		//ť�� ù��° ���ڸ� ������ Ȯ���ϰ�, queue���� �����Ѵ�.
		CUI* pUI = queue.front();
		queue.pop_front();

		//������ UI�� �ڽ� ����Ʈ�� ������ ��, �ڽĸ���Ʈ���� ��ȸ�ϸ� �ٽ� queue�� �����Ѵ�.
		const vector<CUI*>& vecChild = pUI->GetChildUI();
		for (size_t i = 0; i < vecChild.size(); i++)
		{
			queue.push_back(vecChild[i]);
		}

		//������ UI�� ���� Ȯ��
		//�̺�Ʈ �߻� ������ ���콺 ��ǥ���� ���� �޶����Ƿ� ���콺 ��Ȳ�� �Ǵ��Ѵ�
		if (pUI->IsMouseOn())
		{
			//�켱���� UI�� ����������, �� ���� �켱������ ���� �ٸ� UI�� ���� ���,
			// ���� UI�� LbtnDown ���¸� �����Ѵ�.
			if (bLbtnReleased && pPriorityUI != nullptr && pPriorityUI->IsLbtnDown())
			{
				//���� �켱���� UI�� ���콺 ���� ���¸� ���� �����Ѵ�.
				pPriorityUI->m_bLbtnDown = false;
			}
			//���콺�� �ش� UI�� �ö��־���? �ϴ� �ֿ켱���� UI(�̺�Ʈ�� �޾ư� UI) �ĺ��� ���
			pPriorityUI = pUI;

			//��͸� ���� �ڽ��� Ÿ��Ÿ�� �� pPriorityUI�� �����Ѵ�.
		}
		//�ƹ� UI������ ���콺�� �ö��� ���� ���¿��� ���콺�� ������
		else if (bLbtnReleased)
		{
			//UI�� ���콺 ���� ���¸� ���� �����Ѵ�.
			pUI->m_bLbtnDown = false;
		}
	}
	return pPriorityUI;
}