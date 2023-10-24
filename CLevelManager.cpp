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
	//게임 내 존재하는 모든 레벨의 갯수만큼 반복문을 돌며 모든 레벨들을 삭제한다.
	for (size_t i = 0; i < (int)LEVEL_TYPE::END; i++)
	{
		DEL(m_arrLevel[i]);
	}
}


void CLevelManager::init()
{
	//Level 생성
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
	//현재 레벨의 tick을 호출한다.
	m_pCurLevel->tick();

	//Final Tick이 있는 오브젝트들은 final tick을 호출한다.
	m_pCurLevel->final_tick();
}

void CLevelManager::render(HDC _dc)
{
	//현재 레벨의 render을 호출한다.
	m_pCurLevel->render(_dc);
}

CLevel* CLevelManager::GetLevel(UINT _eLevel)
{
	return m_arrLevel[_eLevel];
}

void CLevelManager::ChangeLevel(LEVEL_TYPE _eNextLevel)
{
	//바꾸고자 하는 레벨이 현재 레벨과 동일하다면 오류처리
	assert(m_pCurLevel != m_arrLevel[(UINT)_eNextLevel]);

	//현재 레벨이 존재한다면, 현재 레벨의 Exit 호출
	if (m_pCurLevel != nullptr)
		m_pCurLevel->Exit();

	//현재 레벨을 바꾸고자 하는 레벨로 지정 후 Enter 호출
	m_pCurLevel = m_arrLevel[(UINT)_eNextLevel];
	m_pCurLevel->Enter();
}
