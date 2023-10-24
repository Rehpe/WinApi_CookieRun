#pragma once
#include "CItem.h"
class CBonus :
    public CItem
{
public:
    CBonus();
    ~CBonus();

public:
    CLONE(CBonus);

public:
    void render(HDC _dc);

private:
    virtual void BeginOverlap(CCollider* _pOther);
    virtual void OnOverlap(CCollider* _pOther);
    virtual void EndOverlap(CCollider* _pOther);


}; 


