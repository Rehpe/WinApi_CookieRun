#pragma once

//������ Level(Scene, Stage) ���� ����ִ� ������Ʈ���� �Ѱ��ϴ� Ŭ����

class CLevel;	//Level Ŭ������ �����ؾ��ϹǷ� ���漱�����ش�.

class CLevelManager
{
	SINGLETON(CLevelManager);

	friend class CEventManager;		//EventManager�� ChangeLevel�� ������ �� �ְ� friend ó��

private:
	CLevel*		m_arrLevel[(UINT)LEVEL_TYPE::END];	//���� ���� ��� ������ �����ϴ� �迭
	CLevel*		m_pCurLevel;						//���� Ȱ��ȭ�� ����


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

