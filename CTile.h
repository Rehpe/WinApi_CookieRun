#pragma once
#include "CObj.h"

class CTexture;

class CTile :
    public CObj
{
public:
    CTile();
   ~CTile();

public:
    CLONE(CTile);

private:
    CTexture*       m_pAtlas;       //타일 아틀라스 이미지 포인터
    int             m_iImgIdx;      //아틀라스 내의 몇 번째 타일인지 인덱스

public:
    virtual void tick();
    virtual void render(HDC _dc);

    void Save(FILE* _pFile);
    void Load(FILE* _pFile);

public:
    void SetAtlas(CTexture* _pAtlas) { m_pAtlas = _pAtlas; }
    void SetImgIdx(int _imgidx);
    void AddImgIdx();
};

