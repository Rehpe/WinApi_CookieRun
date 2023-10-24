#pragma once
#include "CEntity.h"

//���ӿ��� �ҷ��ͼ� ����� �̹���, ���� ���� �ڿ�

class CResource :
    public CEntity
{
    friend class CResourceManager;

public:
    CResource();
    virtual ~CResource();

public:
    //���ҽ����� �� �ϳ��� �ҷ��� ����ϴ� ������ Clone�� �ʿ� ����
    CLONE_DEACTIVATE(CResource);

private:
    //��� ���ҽ����� �⺻������ ����/�ҷ����⸦ �� �� �־���ϹǷ�, Ű���� ����θ� ������.
    wstring     m_strKey;           //���ҽ��� Ű��
    wstring     m_strRelativePath;  //���ҽ��� ����/�ε�� ��� ���
    
public:
    const wstring& GetKey() { return m_strKey; }
    const wstring& GetRelativePath() { return m_strRelativePath; }

private:
    //Setting �Լ��� private ó���Ͽ�, ResourceManager������ ������ �����ϴ�
    void SetKey(const wstring& _strKey) { m_strKey = _strKey; }
    void SetRelativePath(const wstring& _strRelativePath) { m_strRelativePath = _strRelativePath; }

protected:
    virtual int Load(const wstring& _strFilePath) = 0;
};

