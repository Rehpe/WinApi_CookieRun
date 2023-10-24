#include "pch.h"
#include "CLevelManager.h"
#include "CLevel.h"
#include "CEditorLevel.h"
#include "CStage1.h"
#include "CBonusStage.h"
#include "CIntroLevel.h"
#include "CSelectLevel.h"
#include "CLoadLevel.h"
#include "CResultLevel.h"

CLevelManager::CLevelManager()
	: m_arrLevel{}
	, m_pCurLevel(nullptr)
{
}

CLevelManager::~CLevelManager()
{
	//���� �� �����ϴ� ��� ������ ������ŭ �ݺ����� ���� ��� �������� �����Ѵ�.
	for (size_t i = 0; i < (int)LEVEL_TYPE::END; i++)
	{
		DEL(m_arrLevel[i]);
	}
}


void CLevelManager::init()
{
	//Level ����
	m_arrLevel[(UINT)LEVEL_TYPE::EDITOR] = new CEditorLevel;
	m_arrLevel[(UINT)LEVEL_TYPE::STAGE_01] = new CStage1;
	m_arrLevel[(UINT)LEVEL_TYPE::BONUS] = new CBonusStage;
	m_arrLevel[(UINT)LEVEL_TYPE::INTRO] = new CIntroLevel;
	m_arrLevel[(UINT)LEVEL_TYPE::SELECT] = new CSelectLevel;
	m_arrLevel[(UINT)LEVEL_TYPE::LOAD] = new CLoadLevel;
	m_arrLevel[(UINT)LEVEL_TYPE::RESULT] = new CResultLevel;


	ChangeLevel(LEVEL_TYPE::INTRO);
}

void CLevelManager::tick()
{
	//���� ������ tick�� ȣ���Ѵ�.
	m_pCurLevel->tick();

	//Final Tick�� �ִ� ������Ʈ���� final tick�� ȣ���Ѵ�.
	m_pCurLevel->final_tick();
}

void CLevelManager::render(HDC _dc)
{
	//���� ������ render�� ȣ���Ѵ�.
	m_pCurLevel->render(_dc);
}

CLevel* CLevelManager::GetLevel(UINT _eLevel)
{
	return m_arrLevel[_eLevel];
}

void CLevelManager::ChangeLevel(LEVEL_TYPE _eNextLevel)
{
	//�ٲٰ��� �ϴ� ������ ���� ������ �����ϴٸ� ����ó��
	assert(m_pCurLevel != m_arrLevel[(UINT)_eNextLevel]);

	//���� ������ �����Ѵٸ�, ���� ������ Exit ȣ��
	if (m_pCurLevel != nullptr)
		m_pCurLevel->Exit();

	//���� ������ �ٲٰ��� �ϴ� ������ ���� �� Enter ȣ��
	m_pCurLevel = m_arrLevel[(UINT)_eNextLevel];
	m_pCurLevel->Enter();
}
