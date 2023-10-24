#pragma once
#include "CMapTile.h"
class CItem :
    public CMapTile
{
public:
    CItem();
    virtual ~CItem();

public:
    CLONE(CItem);

protected:
    CTexture* m_pTexture;

public:
    void render(HDC _dc);

private:
    virtual void BeginOverlap(CCollider* _pOther);
    virtual void OnOverlap(CCollider* _pOther);
    virtual void EndOverlap(CCollider* _pOther);

};

