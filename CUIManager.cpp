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

	//현재 레벨 접근 후 UI Layer 가져오기
	CLevel* pCurLevel = CLevelManager::GetInst()->GetCurLevel();
	const vector<CObj*>& vecUI = pCurLevel->GetLayer(LAYER::UI);

	//제일 마지막 레이어가 최상단에 렌더링되므로 FocusUI라고 가정하고, 벡터의 거꾸로부터 순회
	for (int i = vecUI.size() - 1; i >= 0; i--)
	{
		//부모 본인을 포함하여 부모가 가진 자식 UI중 이벤트를 가져갈 최우선순위 UI를 찾는다.
		m_pPriorityUI = GetPriorityUI((CUI*)vecUI[i]);

		if (m_pPriorityUI == nullptr)
			continue;

		//선발된 최우선순위 UI의 마우스온 이벤트 호출
		m_pPriorityUI->MouseOn();

		//마우스 왼쪽 버튼이 눌렸다
		if (bLbtnPressed)
		{
			//LbtnDown 이벤트 호출(UI를 마우스가 클릭하여 누르고 있는 상태)
			m_pPriorityUI->MouseLbtnDown();

			// 부모UI 를 FocusedUI 로 설정
			m_pFocusedUI = (CUI*)vecUI[i];
			pCurLevel->SetFocusedUI(m_pFocusedUI);
			break;
		}
		//마우스가 떼졌다
		else if (bLbtnReleased)
		{
			//이전에 눌린 적이 있다 (UI를 누르고 있는 상태였다)
			if (m_pPriorityUI->m_bLbtnDown == true)
			{
				//UI를 클릭한 것이다(클릭 이벤트 호출_
				m_pPriorityUI->MouseLbtnClicked();
				m_pPriorityUI->m_bLbtnDown = false;
			}
		}
	}
}

CUI* CUIManager::GetPriorityUI(CUI* _pParentUI)
{
	bool bLbtnReleased = IsRelease(KEY::LBTN);

	//pPRiorityUI 초기화 및 리턴용 변수 생성
	CUI* pPriorityUI = nullptr;

	//queue 지역변수로 선언 후 초기화
	static list<CUI*> queue;
	queue.clear();

	//Queue에 부모 UI 삽입
	queue.push_back(_pParentUI);

	//큐가 빌때까지
	while (!queue.empty())
	{
		//큐의 첫번째 인자를 꺼내서 확인하고, queue에서 삭제한다.
		CUI* pUI = queue.front();
		queue.pop_front();

		//꺼내진 UI의 자식 리스트를 가져온 후, 자식리스트들을 순회하며 다시 queue에 삽입한다.
		const vector<CUI*>& vecChild = pUI->GetChildUI();
		for (size_t i = 0; i < vecChild.size(); i++)
		{
			queue.push_back(vecChild[i]);
		}

		//꺼내온 UI의 상태 확인
		//이벤트 발생 조건이 마우스 좌표값에 따라 달라지므로 마우스 상황을 판단한다
		if (pUI->IsMouseOn())
		{
			//우선순위 UI로 지정됐지만, 더 높은 우선순위를 가진 다른 UI가 나온 경우,
			// 기존 UI의 LbtnDown 상태를 해제한다.
			if (bLbtnReleased && pPriorityUI != nullptr && pPriorityUI->IsLbtnDown())
			{
				//기존 우선순위 UI의 마우스 눌린 상태를 전부 해제한다.
				pPriorityUI->m_bLbtnDown = false;
			}
			//마우스가 해당 UI에 올라가있었다? 일단 최우선순위 UI(이벤트를 받아갈 UI) 후보에 등록
			pPriorityUI = pUI;

			//재귀를 돌며 자식을 타고타고 들어가 pPriorityUI를 갱신한다.
		}
		//아무 UI위에도 마우스가 올라가지 않은 상태에서 마우스가 떼지면
		else if (bLbtnReleased)
		{
			//UI의 마우스 눌린 상태를 전부 해제한다.
			pUI->m_bLbtnDown = false;
		}
	}
	return pPriorityUI;
}