#pragma once
#include "CResource.h"

//게임 내에서 보여질 이미지

class CTexture :
    public CResource
{
    friend class CResourceManager;

public:
    CTexture();
    ~CTexture();

private:
    //텍스쳐는 기본적으로 이미지를 화면에 그려내야하므로 비트맵과 비트맵을 표현할 DC, 비트맵 정보를 가진다.
    HBITMAP     m_hBit;
    HDC         m_hDC;

    BITMAP      m_tBitmapInfo;  //해당 비트맵의 정보

public:
    UINT GetWidth() { return m_tBitmapInfo.bmWidth; }
    UINT GetHeight() { return m_tBitmapInfo.bmHeight; }
    HDC GetDC() { return m_hDC; }

    void Resize(UINT _iWidth, UINT _iHeight);

private:
    virtual int Load(const wstring& _strFilePath) override;

    void Create(UINT _iWidth, UINT _iHeight);       //필요시 자체적으로 텍스처를 제작하는 함수
};

