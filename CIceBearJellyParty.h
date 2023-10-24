#pragma once
#include "CItem.h"
class CIceBearJellyParty :
    public CItem
{
public:
    CIceBearJellyParty();
    ~CIceBearJellyParty();

public:
    CLONE(CIceBearJellyParty);

public:
    void render(HDC _dc);

private:
    virtual void BeginOverlap(CCollider* _pOther);
    virtual void OnOverlap(CCollider* _pOther);
    virtual void EndOverlap(CCollider* _pOther);
};

