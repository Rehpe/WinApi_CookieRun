#pragma once

//리소스들을 저장/로딩 및 관리해주는 매니저 클래스
class CTexture;
class CSound;

class CResourceManager
{
	SINGLETON(CResourceManager);

private:
	//한 번이라도 불러온 리소스는 각 리소스 맵에 저장하여 다시 불러 쓸 수 있게 한다
	map<wstring, CTexture*>		m_mapTex;		//텍스쳐 맵
	map<wstring, CSound*>		m_mapSounds;	//사운드 맵

public:
	CTexture* FindTexture(const wstring& _strKey);
	CTexture* LoadTexture(const wstring& _strKey, const wstring& _strRelativePath);
	
	//경로에서 이미지를 불러오지 않고 자체적으로 단색 비트맵 제작할 수 있는 함수
	CTexture* CreateTexture(const wstring& _strKey, UINT _iWidth, UINT _iHeight);

	CSound* FindSound(const wstring& _strKey);
	CSound* LoadSound(const wstring& _strKey, const wstring& _strRelativePath);
};


