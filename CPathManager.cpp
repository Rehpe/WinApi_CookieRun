#include "pch.h"
#include "CPathManager.h"

CPathManager::CPathManager()
	:m_szContentPath{}
{
}

CPathManager::~CPathManager()
{
}

void CPathManager::init()
{
	//현재 솔루션의 경로를 가져온다
	GetCurrentDirectory(256, m_szContentPath);

	//상위폴더로 한 번 올리는 과정
	//경로의 문자열 길이 반환
	int iLen = wcslen(m_szContentPath);

	//문자열을 거꾸로 순회하며 처음 만나는 '\' 문자를 찾아 공백문자로 치환
	for (int i = iLen - 1; i >= 0; i--)
	{
		// '\'문자의 아스키넘버 = 92
		if (m_szContentPath[i] == 92)
		{
			//공백 문자인 0으로 치환한다
			m_szContentPath[i] = 0;
			break;
		}
	}

	// + \\bin\\content 붙여 리소스 폴더를 가리키게 만든다
	wcscat_s(m_szContentPath, L"\\bin\\content\\");
}

void CPathManager::render()
{
}
