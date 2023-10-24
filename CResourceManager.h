#pragma once

//���ҽ����� ����/�ε� �� �������ִ� �Ŵ��� Ŭ����
class CTexture;
class CSound;

class CResourceManager
{
	SINGLETON(CResourceManager);

private:
	//�� ���̶� �ҷ��� ���ҽ��� �� ���ҽ� �ʿ� �����Ͽ� �ٽ� �ҷ� �� �� �ְ� �Ѵ�
	map<wstring, CTexture*>		m_mapTex;		//�ؽ��� ��
	map<wstring, CSound*>		m_mapSounds;	//���� ��

public:
	CTexture* FindTexture(const wstring& _strKey);
	CTexture* LoadTexture(const wstring& _strKey, const wstring& _strRelativePath);
	
	//��ο��� �̹����� �ҷ����� �ʰ� ��ü������ �ܻ� ��Ʈ�� ������ �� �ִ� �Լ�
	CTexture* CreateTexture(const wstring& _strKey, UINT _iWidth, UINT _iHeight);

	CSound* FindSound(const wstring& _strKey);
	CSound* LoadSound(const wstring& _strKey, const wstring& _strRelativePath);
};


