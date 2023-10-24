#include "pch.h"
#include "CFSM.h"
#include "CState.h"

CFSM::CFSM(CObj* _pOwner)
	:CComponent(_pOwner)
	, m_pCurState(nullptr)
{
}

CFSM::CFSM(const CFSM& _other)
	:CComponent(_other)
	, m_pCurState(_other.m_pCurState)
{
	//_other.m_mapState의 객체들을 반복문을 돌며 깊은 복사하여 복사할 맵에 넣어준다.
	map<wstring, CState*>::const_iterator iter = _other.m_mapState.begin();
	for (; iter != _other.m_mapState.end(); iter++)
	{
		//State 포인터 클론
		CState* pState = iter->second->Clone();
		//State의 소유 애니메이터를 자신으로 설정
		pState->m_pOwnerFSM = this;
		//복사한 애니메이션 맵에 새로 만든 애니메이션 삽입
		m_mapState.insert(make_pair(iter->first, pState));
	}

	//복사가 끝나면 원본 State와 같은 State로 지정시킨다
	ChangeState(_other.m_pCurState->GetKey());
}

CFSM::~CFSM()
{
	map<wstring, CState*>::iterator iter = m_mapState.begin();
	for (; iter != m_mapState.end(); iter++)
	{
		DEL(iter->second);
	}
	m_mapState.clear();
}

void CFSM::tick()
{
}

void CFSM::final_tick()
{
	if (m_pCurState == nullptr)
		return;

	m_pCurState->final_tick();
}

void CFSM::render(HDC _dc)
{
}

void CFSM::AddState(const wstring& _strKey, CState* _pState)
{
	//State 맵에 해당 State를 추가한다
	m_mapState.insert(make_pair(_strKey, _pState));

	//해당 state의 소유 FSM을 본인으로 설정한다.
	_pState->m_pOwnerFSM = this;
	
	//해당 State의 Key값을 저장한다.
	_pState->SetKey(_strKey);
}

CState* CFSM::FindState(const wstring& _strKey)
{
	map<wstring, CState*>::iterator iter = m_mapState.find(_strKey);
	if (iter != m_mapState.end())
		return iter->second;

	return nullptr;

}

void CFSM::ChangeState(const wstring& _strStateName)
{
	CState* pNextState = FindState(_strStateName);
	assert(pNextState);

	//현재 상태 Exit 호출
	if (m_pCurState != nullptr)
		m_pCurState->Exit();

	//현재 상태를 바꿀 상태로 치환
	m_pCurState = pNextState;

	//다음 상태 Enter 호출
	m_pCurState->Enter();
}
