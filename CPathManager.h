#pragma once

//Release / Debug 버전에 상관 없이 리소스 파일을 정상적으로 로드해올 수 있는 경로 담당 클래스

class CPathManager
{
	SINGLETON(CPathManager)

private:
	wchar_t		m_szContentPath[256];	//2바이트 문자열 배열, 경로

public:
	void init();
	void render();

public:
	const wchar_t* GetContentPath() { return m_szContentPath; }
};

