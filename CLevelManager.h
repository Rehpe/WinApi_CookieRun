#pragma once

//게임의 Level(Scene, Stage) 내에 들어있는 오브젝트들을 총괄하는 클래스

class CLevel;	//Level 클래스를 관리해야하므로 전방선언해준다.

class CLevelManager
{
	SINGLETON(CLevelManager);

	friend class CEventManager;		//EventManager가 ChangeLevel에 접근할 수 있게 friend 처리

private:
	CLevel*		m_arrLevel[(UINT)LEVEL_TYPE::END];	//게임 내의 모든 레벨을 관리하는 배열
	CLevel*		m_pCurLevel;						//현재 활성화된 레벨


public:
	void init();
	void tick();
	void render(HDC _dc);

public:
	CLevel* GetCurLevel() { return m_pCurLevel; }
	CLevel* GetLevel(UINT _eLevel);

private:
	void ChangeLevel(LEVEL_TYPE _eNextLevel);

};

