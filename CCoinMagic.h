#pragma once
#include "CItem.h"
class CCoinMagic :
    public CItem
{
public:
    CCoinMagic();
    ~CCoinMagic();

public:
    CLONE(CCoinMagic);

public:
    void render(HDC _dc);

private:
    virtual void BeginOverlap(CCollider* _pOther);
    virtual void OnOverlap(CCollider* _pOther);
    virtual void EndOverlap(CCollider* _pOther);


};
