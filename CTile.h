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
    CTexture*       m_pAtlas;       //Ÿ�� ��Ʋ�� �̹��� ������
    int             m_iImgIdx;      //��Ʋ�� ���� �� ��° Ÿ������ �ε���

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

