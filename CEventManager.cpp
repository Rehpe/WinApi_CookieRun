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
	// 01. ���� ���� ������Ʈ�� ����
	for (size_t i = 0; i < m_vecGarbage.size(); i++)
	{
		delete m_vecGarbage[i];
	}
	//���� �� ���� �ʱ�ȭ
	m_vecGarbage.clear();

	// 02. �̺�Ʈ ó��
	for (size_t i = 0; i < m_vecEvent.size(); i++)
	{
		switch (m_vecEvent[i].eType)
		{
		// ������Ʈ ���� �̺�Ʈ 
		case EVENT_TYPE::CREATE_OBJECT: // wParam : Object Adress, lParam : Layer
		{
			//���� �Ŵ������� ���� ���� �޾ƿ�
			CLevel* pCurLevel = CLevelManager::GetInst()->GetCurLevel();

			CObj* pNewObj = (CObj*)m_vecEvent[i].wParam;	//������ ������Ʈ
			LAYER eLayer = (LAYER)m_vecEvent[i].lParam;		//������Ʈ�� ���̾�

			//�̺�Ʈ ����ü���� ������ ���ڸ� �������� ���� ���� ���Ϳ� ������Ʈ�� �־��ش�.
			pCurLevel->AddObject(pNewObj, eLayer);
		}
			break;

		//������Ʈ ���� �̺�Ʈ
		case EVENT_TYPE::DELETE_OBJECT: // wParam : Object Adress
		{
			//������Ʈ ������ ��� �� �ܰ踦 ��ģ��
			//���� �����ӿ� Dead���·� ����� -> �� ���� �����ӿ� ������ �޸� ���� 

			CObj* pObj = (CObj*)m_vecEvent[i].wParam;		//������ ������Ʈ

			//�ߺ� ������ ���� ����, dead ���°� �ƴ� ��쿡�� ���� ���Ϳ� �߰��Ѵ�.
			if (pObj->m_bDead == false)
			{
				m_vecGarbage.push_back(pObj);
				pObj->m_bDead = true;
			}
		}
			break;
		//���� ���� �̺�Ʈ
		case EVENT_TYPE::LEVEL_CHANGE:	// wParam : Change Level Type
		{
			LEVEL_TYPE eNextLevel = (LEVEL_TYPE)m_vecEvent[i].wParam;	//�ٲ� ����

			CLevelManager::GetInst()->ChangeLevel(eNextLevel);
		}
			break;
		case EVENT_TYPE::STATE_CHANGE:	// Wparam: FSM Component �ּ�, lParam : �ٲ� State �̸�
		{
			CFSM* pFSM = (CFSM*)m_vecEvent[i].wParam;
			const wchar_t* pName = (const wchar_t*)m_vecEvent[i].lParam;

			pFSM->ChangeState(pName);
		}

		default:
			break;
		}
	}

	//�̺�Ʈ�� ó���ߴٸ� ���Ϳ��� ����.
	m_vecEvent.clear();
}

