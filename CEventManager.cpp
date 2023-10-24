#include "pch.h"
#include "CEventManager.h"
#include "CLevel.h"
#include "CLevelManager.h"
#include "CObj.h"

#include "CFSM.h"
#include "CState.h"

CEventManager::CEventManager()
	: m_vecEvent{}
{
}

CEventManager::~CEventManager()
{
}

void CEventManager::tick()
{
	// 01. 삭제 예정 오브젝트들 삭제
	for (size_t i = 0; i < m_vecGarbage.size(); i++)
	{
		delete m_vecGarbage[i];
	}
	//삭제 후 벡터 초기화
	m_vecGarbage.clear();

	// 02. 이벤트 처리
	for (size_t i = 0; i < m_vecEvent.size(); i++)
	{
		switch (m_vecEvent[i].eType)
		{
		// 오브젝트 생성 이벤트 
		case EVENT_TYPE::CREATE_OBJECT: // wParam : Object Adress, lParam : Layer
		{
			//레벨 매니저에서 현재 레벨 받아옴
			CLevel* pCurLevel = CLevelManager::GetInst()->GetCurLevel();

			CObj* pNewObj = (CObj*)m_vecEvent[i].wParam;	//생성할 오브젝트
			LAYER eLayer = (LAYER)m_vecEvent[i].lParam;		//오브젝트의 레이어

			//이벤트 구조체에서 가져온 인자를 바탕으로 현재 레벨 벡터에 오브젝트를 넣어준다.
			pCurLevel->AddObject(pNewObj, eLayer);
		}
			break;

		//오브젝트 삭제 이벤트
		case EVENT_TYPE::DELETE_OBJECT: // wParam : Object Adress
		{
			//오브젝트 삭제의 경우 두 단계를 거친다
			//다음 프레임에 Dead상태로 만들고 -> 그 다음 프레임에 실제로 메모리 삭제 

			CObj* pObj = (CObj*)m_vecEvent[i].wParam;		//삭제할 오브젝트

			//중복 삭제를 막기 위해, dead 상태가 아닐 경우에만 삭제 벡터에 추가한다.
			if (pObj->m_bDead == false)
			{
				m_vecGarbage.push_back(pObj);
				pObj->m_bDead = true;
			}
		}
			break;
		//레벨 변경 이벤트
		case EVENT_TYPE::LEVEL_CHANGE:	// wParam : Change Level Type
		{
			LEVEL_TYPE eNextLevel = (LEVEL_TYPE)m_vecEvent[i].wParam;	//바꿀 레벨

			CLevelManager::GetInst()->ChangeLevel(eNextLevel);
		}
			break;
		case EVENT_TYPE::STATE_CHANGE:	// Wparam: FSM Component 주소, lParam : 바꿀 State 이름
		{
			CFSM* pFSM = (CFSM*)m_vecEvent[i].wParam;
			const wchar_t* pName = (const wchar_t*)m_vecEvent[i].lParam;

			pFSM->ChangeState(pName);
		}

		default:
			break;
		}
	}

	//이벤트를 처리했다면 벡터에서 뺀다.
	m_vecEvent.clear();
}

