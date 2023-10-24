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
	//���� �ַ���� ��θ� �����´�
	GetCurrentDirectory(256, m_szContentPath);

	//���������� �� �� �ø��� ����
	//����� ���ڿ� ���� ��ȯ
	int iLen = wcslen(m_szContentPath);

	//���ڿ��� �Ųٷ� ��ȸ�ϸ� ó�� ������ '\' ���ڸ� ã�� ���鹮�ڷ� ġȯ
	for (int i = iLen - 1; i >= 0; i--)
	{
		// '\'������ �ƽ�Ű�ѹ� = 92
		if (m_szContentPath[i] == 92)
		{
			//���� ������ 0���� ġȯ�Ѵ�
			m_szContentPath[i] = 0;
			break;
		}
	}

	// + \\bin\\content �ٿ� ���ҽ� ������ ����Ű�� �����
	wcscat_s(m_szContentPath, L"\\bin\\content\\");
}

void CPathManager::render()
{
}
