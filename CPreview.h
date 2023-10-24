#pragma once
#include "CObj.h"

class CTexture;

class CPreview :
    public CObj
{
public:
    CPreview();
    ~CPreview();
    
public:
    CLONE_DEACTIVATE(CPreview);

private:
    CTexture* m_pPreviewTex;

public:
    void render(HDC _dc);

public:
    void SetPreviewTexture(CTexture* _pTexture) { m_pPreviewTex = _pTexture; }
    CTexture* GetPreviewTexture() { return m_pPreviewTex; }
};

