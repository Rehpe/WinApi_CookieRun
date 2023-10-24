#pragma once
#include "CItem.h"
class CMiniPotion :
    public CItem
{
public:
    CMiniPotion();
    ~CMiniPotion();

public:
    CLONE(CMiniPotion);

public:
    void render(HDC _dc);

private:
    virtual void BeginOverlap(CCollider* _pOther);
    virtual void OnOverlap(CCollider* _pOther);
    virtual void EndOverlap(CCollider* _pOther);

};

