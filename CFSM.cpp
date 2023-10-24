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
	//_other.m_mapState�� ��ü���� �ݺ����� ���� ���� �����Ͽ� ������ �ʿ� �־��ش�.
	map<wstring, CState*>::const_iterator iter = _other.m_mapState.begin();
	for (; iter != _other.m_mapState.end(); iter++)
	{
		//State ������ Ŭ��
		CState* pState = iter->second->Clone();
		//State�� ���� �ִϸ����͸� �ڽ����� ����
		pState->m_pOwnerFSM = this;
		//������ �ִϸ��̼� �ʿ� ���� ���� �ִϸ��̼� ����
		m_mapState.insert(make_pair(iter->first, pState));
	}

	//���簡 ������ ���� State�� ���� State�� ������Ų��
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
	//State �ʿ� �ش� State�� �߰��Ѵ�
	m_mapState.insert(make_pair(_strKey, _pState));

	//�ش� state�� ���� FSM�� �������� �����Ѵ�.
	_pState->m_pOwnerFSM = this;
	
	//�ش� State�� Key���� �����Ѵ�.
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

	//���� ���� Exit ȣ��
	if (m_pCurState != nullptr)
		m_pCurState->Exit();

	//���� ���¸� �ٲ� ���·� ġȯ
	m_pCurState = pNextState;

	//���� ���� Enter ȣ��
	m_pCurState->Enter();
}
