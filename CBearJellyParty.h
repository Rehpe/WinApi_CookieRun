#pragma once
#include "CItem.h"
class CBearJellyParty :
    public CItem
{
public:
    CBearJellyParty();
    ~CBearJellyParty();

public:
    CLONE(CBearJellyParty);

public:
    void render(HDC _dc);

private:
    virtual void BeginOverlap(CCollider* _pOther);
    virtual void OnOverlap(CCollider* _pOther);
    virtual void EndOverlap(CCollider* _pOther);


};

