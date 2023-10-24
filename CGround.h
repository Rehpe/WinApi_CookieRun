#pragma once
#include "CMapTile.h"

class CGround :
    public CMapTile
{
public:
    CGround();
    virtual ~CGround();

public:
    CLONE(CGround);

public:
    void render(HDC _dc);

private:
    virtual void BeginOverlap(CCollider* _pOther);
    virtual void OnOverlap(CCollider* _pOther);
    virtual void EndOverlap(CCollider* _pOther);

  
};

