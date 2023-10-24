#include "pch.h"
#include "CState.h"
#include "CEventManager.h"

CState::CState()
	: m_pOwnerFSM(nullptr)
{
}

CState::~CState()
{
}

void CState::final_tick()
{
}

void CState::Enter()
{
}

void CState::Exit()
{
}

void CState::ChangeState(const wchar_t* _strStateName)
{
	//State를 변경하는 작업은 이벤트 매니저를 거쳐서 진행된다.

	tEvent evn = {};

	evn.eType = EVENT_TYPE::STATE_CHANGE;		//이벤트 타입
	evn.wParam = (DWORD_PTR)GetOwnerFSM();		//wParam: 이 State를 가진 FSM
	evn.lParam = (DWORD_PTR)_strStateName;		//lParam: 바꿀 State Name

	CEventManager::GetInst()->AddEvent(evn);
}
