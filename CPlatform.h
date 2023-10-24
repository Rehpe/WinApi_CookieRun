#pragma once
#include "CMapTile.h"
#include "CGround.h"
class CPlatform :
    public CMapTile
{
public:
    CPlatform();
    ~CPlatform();

public:
    CLONE(CPlatform);

public:
    virtual void render(HDC _dc);

private:
    virtual void BeginOverlap(CCollider* _pOther);
    virtual void OnOverlap(CCollider* _pOther);
    virtual void EndOverlap(CCollider* _pOther);

};


