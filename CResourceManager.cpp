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
	//������ �ִ� ���ҽ� ���� �� ��ü�� ��ȸ�ϸ� ���ҽ����� �����Ѵ�.
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
	//�ؽ�ó �ʿ� Ű���� �־� �ؽ��ĸ� ã�´�.
	map<wstring, CTexture*>::iterator iter = m_mapTex.find(_strKey);

	//ã�� ���ߴٸ� nullptr ��ȯ
	if (iter == m_mapTex.end())
		return nullptr;
	
	//ã�Ҵٸ� �ؽ��� ��ȯ(first�� Ű��, second�� �ؽ��� ������)
	return iter->second;
}

CTexture* CResourceManager::LoadTexture(const wstring& _strKey, const wstring& _strRelativePath)
{
	//�̹� �� �� �ε�� �� �ִ� ���ҽ����� �ʿ� �˻��غ���.
	CResource* pTexture = FindTexture(_strKey);

	//ã�Ҵٸ�, CTexture�� �ٿ�ĳ�����Ͽ� ���ҽ��� �����Ѵ�.
	if (pTexture != nullptr)
		return (CTexture*)pTexture;

	//�ε�� ���� ���� ���ҽ����(nullptr�̶��) PathManager�� �̿��Ͽ� �ش� �ؽ��ĸ� �ε��Ѵ�.
	wstring strFilePath = CPathManager::GetInst()->GetContentPath();
	strFilePath += _strRelativePath;

	//�ε��ؿ� �ؽ��� ���� ����
	pTexture = new CTexture;
	pTexture->Load(strFilePath);
	pTexture->SetKey(_strKey);
	pTexture->SetRelativePath(_strRelativePath);
	
	//������ ������ �������� �ؽ��� ���ҽ� �ʿ� ����Ѵ�.
	m_mapTex.insert(make_pair(_strKey, (CTexture*)pTexture));

	return (CTexture*)pTexture;
}


CTexture* CResourceManager::CreateTexture(const wstring& _strKey, UINT _iWidth, UINT _iHeight)
{
	//�̹� �� �� �ε�� �� �ִ� ���ҽ����� �ʿ� �˻��غ���.
	CTexture* pTexture = FindTexture(_strKey);

	//ã�Ҵٸ�, CTexture�� �ٿ�ĳ�����Ͽ� ���ҽ��� �����Ѵ�.
	if (pTexture != nullptr)
		return (CTexture*)pTexture;

	//���ٸ� ���� �ؽ��ĸ� �����.
	pTexture = new CTexture;
	pTexture->Create(_iWidth, _iHeight);
	pTexture->SetKey(_strKey);

	//������ �ؽ��ĸ� �ʿ� ����صд�.
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

	// PathMgr �� �̿��ؼ� ���� Texture�� ��θ� �����.
	wstring strFilePath = CPathManager::GetInst()->GetContentPath();
	strFilePath += _strRelativePath;

	// �ؽ��� ���� �� �ε�
	pSound = new CSound;
	pSound->Load(strFilePath);
	pSound->SetKey(_strKey);
	pSound->SetRelativePath(_strRelativePath);

	// Map �� ����
	m_mapSounds.insert(make_pair(_strKey, (CSound*)pSound));

	return (CSound*)pSound;
}
