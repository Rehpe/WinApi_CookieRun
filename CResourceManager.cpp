#include "pch.h"
#include "CResourceManager.h"
#include "CResource.h"
#include "CTexture.h"
#include "CPathManager.h"
#include "CEngine.h"
#include "CSound.h"

CResourceManager::CResourceManager()
{
}

CResourceManager::~CResourceManager()
{
	//가지고 있는 리소스 관리 맵 전체를 순회하며 리소스들을 삭제한다.
	map<wstring, CTexture*>::iterator iter = m_mapTex.begin();
	for (; iter != m_mapTex.end(); iter++)
	{
		delete iter->second;
	}

	map<wstring, CSound*>::iterator iter_sound = m_mapSounds.begin();
	for (; iter_sound != m_mapSounds.end(); iter_sound++)
	{
		delete iter_sound->second;
	}
}

CTexture* CResourceManager::FindTexture(const wstring& _strKey)
{
	//텍스처 맵에 키값을 넣어 텍스쳐를 찾는다.
	map<wstring, CTexture*>::iterator iter = m_mapTex.find(_strKey);

	//찾지 못했다면 nullptr 반환
	if (iter == m_mapTex.end())
		return nullptr;
	
	//찾았다면 텍스쳐 반환(first는 키값, second는 텍스쳐 포인터)
	return iter->second;
}

CTexture* CResourceManager::LoadTexture(const wstring& _strKey, const wstring& _strRelativePath)
{
	//이미 한 번 로드된 적 있는 리소스인지 맵에 검색해본다.
	CResource* pTexture = FindTexture(_strKey);

	//찾았다면, CTexture로 다운캐스팅하여 리소스를 리턴한다.
	if (pTexture != nullptr)
		return (CTexture*)pTexture;

	//로드된 적이 없는 리소스라면(nullptr이라면) PathManager을 이용하여 해당 텍스쳐를 로드한다.
	wstring strFilePath = CPathManager::GetInst()->GetContentPath();
	strFilePath += _strRelativePath;

	//로드해온 텍스쳐 정보 세팅
	pTexture = new CTexture;
	pTexture->Load(strFilePath);
	pTexture->SetKey(_strKey);
	pTexture->SetRelativePath(_strRelativePath);
	
	//세팅한 정보를 바탕으로 텍스쳐 리소스 맵에 등록한다.
	m_mapTex.insert(make_pair(_strKey, (CTexture*)pTexture));

	return (CTexture*)pTexture;
}


CTexture* CResourceManager::CreateTexture(const wstring& _strKey, UINT _iWidth, UINT _iHeight)
{
	//이미 한 번 로드된 적 있는 리소스인지 맵에 검색해본다.
	CTexture* pTexture = FindTexture(_strKey);

	//찾았다면, CTexture로 다운캐스팅하여 리소스를 리턴한다.
	if (pTexture != nullptr)
		return (CTexture*)pTexture;

	//없다면 새로 텍스쳐를 만든다.
	pTexture = new CTexture;
	pTexture->Create(_iWidth, _iHeight);
	pTexture->SetKey(_strKey);

	//생성한 텍스쳐를 맵에 등록해둔다.
	m_mapTex.insert(make_pair(_strKey, pTexture));

	return pTexture;
}

CSound* CResourceManager::FindSound(const wstring& _strKey)
{
	map<wstring, CSound*>::iterator iter = m_mapSounds.find(_strKey);
	if (iter == m_mapSounds.end())
	{
		return nullptr;
	}

	return iter->second;
}

CSound* CResourceManager::LoadSound(const wstring& _strKey, const wstring& _strRelativePath)
{
	CResource* pSound = FindSound(_strKey);

	if (nullptr != pSound)
		return (CSound*)pSound;

	// PathMgr 를 이용해서 최종 Texture의 경로를 만든다.
	wstring strFilePath = CPathManager::GetInst()->GetContentPath();
	strFilePath += _strRelativePath;

	// 텍스쳐 생성 및 로딩
	pSound = new CSound;
	pSound->Load(strFilePath);
	pSound->SetKey(_strKey);
	pSound->SetRelativePath(_strRelativePath);

	// Map 에 저장
	m_mapSounds.insert(make_pair(_strKey, (CSound*)pSound));

	return (CSound*)pSound;
}
