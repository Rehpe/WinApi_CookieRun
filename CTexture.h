#pragma once
#include "CResource.h"

//���� ������ ������ �̹���

class CTexture :
    public CResource
{
    friend class CResourceManager;

public:
    CTexture();
    ~CTexture();

private:
    //�ؽ��Ĵ� �⺻������ �̹����� ȭ�鿡 �׷������ϹǷ� ��Ʈ�ʰ� ��Ʈ���� ǥ���� DC, ��Ʈ�� ������ ������.
    HBITMAP     m_hBit;
    HDC         m_hDC;

    BITMAP      m_tBitmapInfo;  //�ش� ��Ʈ���� ����

public:
    UINT GetWidth() { return m_tBitmapInfo.bmWidth; }
    UINT GetHeight() { return m_tBitmapInfo.bmHeight; }
    HDC GetDC() { return m_hDC; }

    void Resize(UINT _iWidth, UINT _iHeight);

private:
    virtual int Load(const wstring& _strFilePath) override;

    void Create(UINT _iWidth, UINT _iHeight);       //�ʿ�� ��ü������ �ؽ�ó�� �����ϴ� �Լ�
};

