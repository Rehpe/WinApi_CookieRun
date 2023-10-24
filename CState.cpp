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
	//State�� �����ϴ� �۾��� �̺�Ʈ �Ŵ����� ���ļ� ����ȴ�.

	tEvent evn = {};

	evn.eType = EVENT_TYPE::STATE_CHANGE;		//�̺�Ʈ Ÿ��
	evn.wParam = (DWORD_PTR)GetOwnerFSM();		//wParam: �� State�� ���� FSM
	evn.lParam = (DWORD_PTR)_strStateName;		//lParam: �ٲ� State Name

	CEventManager::GetInst()->AddEvent(evn);
}
