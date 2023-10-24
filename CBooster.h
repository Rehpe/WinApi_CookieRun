#pragma once
#include "CItem.h"
class CBooster :
    public CItem
{
public:
    CBooster();
    ~CBooster();

public:
    CLONE(CBooster);

public:
    void render(HDC _dc);

private:
    virtual void BeginOverlap(CCollider* _pOther);
    virtual void OnOverlap(CCollider* _pOther);
    virtual void EndOverlap(CCollider* _pOther);


};

