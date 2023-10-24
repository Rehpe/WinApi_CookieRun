#pragma once
#include "CItem.h"

class CBigger :
    public CItem
{
public:
    CBigger();
    ~CBigger();

public:
    CLONE(CBigger);

public:
    void render(HDC _dc);

private:
    virtual void BeginOverlap(CCollider* _pOther);
    virtual void OnOverlap(CCollider* _pOther);
    virtual void EndOverlap(CCollider* _pOther);


};

