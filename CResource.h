#pragma once
#include "CEntity.h"

//게임에서 불러와서 사용할 이미지, 사운드 등의 자원

class CResource :
    public CEntity
{
    friend class CResourceManager;

public:
    CResource();
    virtual ~CResource();

public:
    //리소스들은 딱 하나만 불러와 사용하는 것으로 Clone이 필요 없다
    CLONE_DEACTIVATE(CResource);

private:
    //모든 리소스들은 기본적으로 저장/불러오기를 할 수 있어야하므로, 키값과 상대경로를 가진다.
    wstring     m_strKey;           //리소스의 키값
    wstring     m_strRelativePath;  //리소스가 저장/로드될 상대 경로
    
public:
    const wstring& GetKey() { return m_strKey; }
    const wstring& GetRelativePath() { return m_strRelativePath; }

private:
    //Setting 함수는 private 처리하여, ResourceManager에서만 접근이 가능하다
    void SetKey(const wstring& _strKey) { m_strKey = _strKey; }
    void SetRelativePath(const wstring& _strRelativePath) { m_strRelativePath = _strRelativePath; }

protected:
    virtual int Load(const wstring& _strFilePath) = 0;
};

