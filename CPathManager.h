#pragma once

//Release / Debug ������ ��� ���� ���ҽ� ������ ���������� �ε��ؿ� �� �ִ� ��� ��� Ŭ����

class CPathManager
{
	SINGLETON(CPathManager)

private:
	wchar_t		m_szContentPath[256];	//2����Ʈ ���ڿ� �迭, ���

public:
	void init();
	void render();

public:
	const wchar_t* GetContentPath() { return m_szContentPath; }
};

